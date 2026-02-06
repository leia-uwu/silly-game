// SPDX-License-Identifier: CC0-1.0
// SPDX-FileCopyrightText: 2026 firecat2d developers

import { defineConfig } from "vite";

export default defineConfig({
    base: "",
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
