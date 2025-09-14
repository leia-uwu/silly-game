import { defineConfig } from "vite";

export default defineConfig({
    build: {
        target: "es2022",
        chunkSizeWarningLimit: 1000,
    },
    server: {
        port: 3000,
        host: "0.0.0.0",
    },
    preview: {
        port: 3000,
        host: "0.0.0.0",
    },
});
