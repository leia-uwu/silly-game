#include "Loop.h"
#include "WebSocket.h"
#include "systems/math/collision.h"
#include "systems/math/shape.h"
#include "systems/math/vec2.h"
#include "systems/net/bytestream.h"
#include "systems/util/formatter.h"

#include <chrono>
#include <cmath>
#include <iostream>

#include <uv.h>

#include <App.h>

static std::chrono::duration<double> LAST_NOW = std::chrono::system_clock::now().time_since_epoch();

static void timer_cb(uv_timer_t* /*unused*/)
{
    std::chrono::duration<double> newNow = std::chrono::system_clock::now().time_since_epoch();
    double dt = std::chrono::duration<double, std::ratio<1>>(newNow - LAST_NOW).count();
    LAST_NOW = newNow;

    // std::cout << 1.0 / dt << "\n";
}

int main()
{
    std::cout << "be " << FORMATTER.bold().underline("gay") << " do " << FORMATTER.red().italic()("crime") << '\n'
              << "(boilerplate message lol)\n";

    std::cout << FORMATTER.blue_bright("n" + FORMATTER.magenta_bright("y" + FORMATTER.white_bright("abo") + "o") + "m") << '\n';

    uv_loop_t* loop = uv_default_loop();
    uv_timer_t tick;

    uv_timer_init(loop, &tick);
    uv_timer_start(&tick, timer_cb, 0, 10);

    struct PerSocketData
    {
        /* Fill with user data */
    };

    uWS::Loop::get(loop);

    /* Keep in mind that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support.
     * You may swap to using uWS:App() if you don't need SSL */
    auto app = uWS::App();

    using wsType = uWS::WebSocket<false, true, PerSocketData>*;

    app.ws<PerSocketData>(
        "/ws",
        {
            .compression = uWS::CompressOptions(uWS::DEDICATED_COMPRESSOR | uWS::DEDICATED_DECOMPRESSOR),
            .maxPayloadLength = 100 * 1024 * 1024,
            .idleTimeout = 16,
            .maxBackpressure = 100 * 1024 * 1024,
            .closeOnBackpressureLimit = false,
            .resetIdleTimeoutOnSend = false,
            .sendPingsAutomatically = true,
            /* Handlers */
            .upgrade = nullptr,
            .open = [](wsType ws) {
                ws->send("hi");
            },
            .message = [](wsType ws, std::string_view message, uWS::OpCode opCode) {
                auto* data = (uint8_t*)(message.data());

                auto bs = ByteStream(data, message.size());

                // std::cout << (int)bs.read_uint8() << "\n";

                ws->send(message, opCode, false);
            },
            .dropped = [](wsType /*ws*/, std::string_view /*message*/, uWS::OpCode /*opCode*/) {

            },
            .drain = [](wsType /*ws*/) {

            },
            .ping = [](wsType /*ws*/, std::string_view) {

            },
            .pong = [](wsType /*ws*/, std::string_view) {

            },
            .close = [](wsType /*ws*/, int /*code*/, std::string_view /*message*/) {

            },
        }
    );

    app.get("/test", [](auto* res, auto* /*req*/) {
        res->end("Hello world!");
    });

    app.listen(9001, [](auto* listen_socket) {
        if (listen_socket) {
            std::cout << "Listening on port " << 9001 << "\n";
        }
    });

    return uv_run(loop, UV_RUN_DEFAULT);
}
