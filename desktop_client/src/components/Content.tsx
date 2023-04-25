import { useEffect, useState } from "react";
import { SerialPort } from "serialport";
import GBPrinter from "./GBPrinter";

// const ref = {
//     path: "/dev/tty.usbmodem14101",
//     manufacturer: "Raspberry Pi",
//     serialNumber: "E6616407E369512A",
//     locationId: "14100000",
//     vendorId: "2e8a",
//     productId: "000a",
// };

type SerialPortType = {
  path?: string | null;
  manufacturer?: string | null;
  serialNumber?: string | null;
  locationId?: string | null;
  vendorId?: string | null;
  productId?: string | null;
};

const Content = ({
  page,
  pages,
}: {
  page: number;
  pages: { id: number; name: string }[];
}) => {
  const [prevPortData, setPrevPortData] = useState<
    SerialPortType | undefined
  >();
  const [portData, setPortData] = useState<SerialPortType | undefined>();
  const [port, setPort] = useState<SerialPort<unknown> | undefined>();

  useEffect(() => {
    const intervalId = setInterval(() => {
      SerialPort.list().then((ports: SerialPortType[]) => {
        const picoPort = ports.find(
          (port) =>
            port.manufacturer == "Raspberry Pi" ||
            (port.vendorId == "2e8a" && port.productId == "000a")
        );

        setPortData(picoPort);
      });
    }, 5000);

    return () => clearInterval(intervalId); //This is important
  }, []);

  useEffect(() => {
    const sameDevice =
      portData &&
      prevPortData &&
      Object.keys(portData)
        .map((pData: string) => {
          const pKey = pData as keyof SerialPortType;
          return portData[pKey] == prevPortData[pKey];
        })
        .reduce((acc: boolean, current: boolean) => {
          if (typeof acc == "undefined" || acc == null) {
            return current;
          }
          return acc && current;
        });

    if (!sameDevice) {
      setPrevPortData(portData);

      if (portData) {
        const p: SerialPort<unknown> = new SerialPort({
          path: portData.path,
          baudRate: 9600,
        });
        setPort(p);
      } else setPort(undefined);
    }
  }, [prevPortData, portData]);

  return (
    <div className="content">
      <div id="error"></div>
      <div id="ports">
        <h3 className="connection">
          {port
            ? "GBTool Connected!"
            : portData
            ? "Attempting to connect"
            : "Waiting for GB Tool to be connected..."}
        </h3>
      </div>
      <div className="inner-content">
        {pages[page].id == 0 && <GBPrinter port={port} />}
        </div>
    </div>
  );
};

export default Content;
