import { useEffect, useRef, useState } from "react";
import { SerialPort } from "serialport";

const GB_PRINTER_WIDTH = 160;

type Dimensions = {
  w: number;
  h: number;
};
const getImageData = (file: string): Promise<HTMLImageElement> => {
  return new Promise(function (resolved, rejected) {
    const i = new Image();
    i.onload = function () {
      resolved(i);
    };
    i.src = file;
  });
};

const GBPrinter = ({ port }: { port: SerialPort<unknown> | undefined }) => {
  const [pendingWrite, setPendingWrite] = useState<boolean>(false);
  const [writing, setWriting] = useState<boolean>(false);
  const [prevWriteBuffer, setPrevWriteBuffer] = useState<Buffer>();
  const [writeBuffer, setWriteBuffer] = useState<Buffer>();

  /**
   * Image Loader
   */
  const [imageData, setImageData] = useState<HTMLImageElement | null>(null);
  const [image, setImage] = useState<string | null>(null);
  const [imageSize, setImageSize] = useState<Dimensions | null>(null);

  const onImageChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    if (event.target.files && event.target.files[0]) {
      const img = event.target.files[0];
      const img_str = URL.createObjectURL(img);
      setImage(img_str);
      getImageData(img_str)
        .then((img_data: HTMLImageElement) => {
          setImageData(img_data);
          const dimensions = {
            w: img_data.width,
            h: img_data.height,
          };
          setImageSize(dimensions);
        })
        .catch(() => {
          setImageData(null);
          setImageSize(null);
        });
    }
  };

  /**
   * Canvas
   */
  const width = GB_PRINTER_WIDTH;
  const height = imageSize ? (width / imageSize.w) * imageSize.h : 144;
  const pixelRatio = window.devicePixelRatio;
  const dw = Math.floor(width);
  const dh = Math.floor(height);
  const style = {
    width: pixelRatio * dw,
    height: pixelRatio * dh,
  };

  const canvas = useRef<HTMLCanvasElement>(null);

  useEffect(() => {
    const context = canvas.current?.getContext("2d");

    if (context && imageData) {
      if (image) {
        context.drawImage(imageData, 0, 0, dw, dh);
        const imgData = context.getImageData(0, 0, dw, dh);
        const pixels = imgData.data;

        // NOTE:
        // Initializing this in one line causes all rows to be references to the same array,
        // meaning every row will be the same
        const pixel_matrix: number[][] = [];
        for (let x = 0; x < dw; x++) {
          pixel_matrix.push(Array(dh).fill(0));
        }

        for (let i = 0; i < pixels.length; i += 4) {
          const lightness = ~~((pixels[i] + pixels[i + 1] + pixels[i + 2]) / 3);
          const four_bit_tone =
            pixels[i + 3] == 0x00
              ? 0xff
              : Math.max(Math.min(Math.round(lightness / 85) * 85, 0xff), 0x00);

          pixels[i] = four_bit_tone;
          pixels[i + 1] = four_bit_tone;
          pixels[i + 2] = four_bit_tone;
          pixels[i + 3] = 0xff;

          // The Gameboy is inverted where 0x00 = White, 0xFF = Black,
          // but it'll be in 2 bit color where 0b00 = White, 0b11 = Black
          const gameboy_pixel_color = 0xff - four_bit_tone;
          const gameboy_pixel = Math.max(
            Math.min(Math.round(gameboy_pixel_color / 85), 3),
            0
          );

          const local_index = i / 4;
          const x = local_index % dw;
          const y = Math.floor(local_index / dw);

          pixel_matrix[x][y] = gameboy_pixel;
        }

        // Go ahead and load image while we prep the image
        context.putImageData(imgData, 0, 0);

        setPendingWrite(true);
        // I broke out the matrix iteration because it's a massive pain
        // to work with a 1D array with a i*4 offset as a 2D array.
        // Trust me, the math is possible, but it ain't easy or maintainable
        //
        // The GB Printer does not read pixels left to right ro by row... unfortunately...
        // Tiles are 8x8 and are serialized top to bottom, left to right
        const tiles: number[] = [];
        for (let y = 0; y < dh; y += 8) {
          for (let x = 0; x < dw; x += 8) {
            for (let innerY = y; innerY < y + 8; innerY++) {
              let tile = 0x00;

              for (let innerX = x; innerX < x + 8; innerX++) {
                const x_index = innerX - x;
                const bit_offset = x_index % 4;

                // If undefined, count it as empty white space
                // This will happen with images that aren't an exact height/y multiple of 16
                const pixel_val = pixel_matrix[innerX][innerY];

                // Pack 4 2-bit nibbles into a byte
                const packable_bin =
                  (pixel_val & 0b11) << ((3 - bit_offset) * 2);
                tile = tile | packable_bin;

                if (bit_offset == 3) {
                  tiles.push(tile);
                  tile = 0x00;
                }
              }
            }
          }
        }

        if (tiles.length > 0) {
          const buffer_data = Buffer.from(tiles);
          setWriteBuffer(buffer_data);
        }
      }
    }
  }, [dh, dw, image, imageData]);

  useEffect(() => {
    if (
      !writing &&
      port &&
      port.writable &&
      writeBuffer &&
      (!prevWriteBuffer || !Buffer.compare(writeBuffer, prevWriteBuffer))
    ) {
      setWriting(true);

      // Convert to hex string
      // This is to avoid null bytes in stdin/stdout
      // This should only double the data size and avoid that problem
      let bd = "";
      writeBuffer.forEach((p) => {
          const str = p.toString(16).padStart(2, "0");
          bd += str;
      });
      // 0x69 0x45 are the terminate bytes
      const stringBuffer = Buffer.from(bd + "6945", "ascii")
      console.log("Writing  bytes...", stringBuffer);

      port.write(stringBuffer, (err: Error | null | undefined) => {
        if (err) {
          console.log("ERROR WRITING", err);
          setWriting(false);
          setPendingWrite(false);
          return;
        }
        // TODO:
        // TEMP Delay. As little as 400ms is needed, 
        // but I need to find a blocking READ that waits for a terminate byte
        setTimeout(() => {
            setWriting(false);
            const read_data = port.read(); // TODO: Determine if working
            console.log("read_data", read_data);
            setPendingWrite(false);
            setPrevWriteBuffer(stringBuffer);
        }, 3000)
      });
    } else {
      console.log("Port wasn't ready...");
    }
  }, [port, writeBuffer]);

  return (
    <div>
      <input type="file" onChange={onImageChange} className="filetype" />
      {imageSize && (
        <>
          <h5>Width: {imageSize.w}</h5>
          <h5>Height: {imageSize.h}</h5>
          <h5>Canvas Width: {canvas.current?.width}</h5>
          <h5>Canvas Height: {canvas.current?.height}</h5>
        </>
      )}
      {image && (
        <img
          src={image}
          style={{
            ...style,
            // Overwriting the style temporarily... unless it becomes permanent
            width: "calc(50% - 6px)",
            height: "unset",
            marginRight: "10px",
          }}
        />
      )}
      <canvas ref={canvas} width={dw} height={dh} style={{
        ...style,
        // Overwriting the style temporarily... unless it becomes permanent
        width: "calc(50% - 6px)",
        height: "unset",
      }} />
    </div>
  );
};

export default GBPrinter;
