import { defineConfig } from "vite";
import electron from "vite-plugin-electron";
import renderer from "vite-plugin-electron-renderer";
import react from "@vitejs/plugin-react-swc";

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [
    // electron({
    //   // renderer: {

    //   // }
    // }),
    renderer({
      resolve: {
        // serialport: { platform: 'node' },
        serialport: { type: 'cjs' },
        // Equivalent to
        // serialport: () => `const lib = require("serialport");\nexport default lib.default || lib;`
      },
    }),
    react(),
  ],
  // build: {
  //   minify: false,
  //   rollupOptions: {
  //     external: ""
  //   }
  // },
});
