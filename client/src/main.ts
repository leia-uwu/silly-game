window.addEventListener("keydown", (event) => {
    if (event.ctrlKey || event.altKey || event.metaKey || event.key.match(/F\d+/ig)) {
        event.stopImmediatePropagation();
    }
}, true);

window.addEventListener("keyup", (event) => {
    if (event.ctrlKey || event.altKey || event.metaKey || event.key.match(/F\d+/ig)) {
        event.stopImmediatePropagation();
    }
}, true);

import "../build/game_client.js"
