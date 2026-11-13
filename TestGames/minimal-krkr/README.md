# Minimal KiriKiri game

`startup.tjs` logs one line to the engine console. That is enough to prove the
TJS VM runs inside the app: the line shows up in the module log right after the
engine reports its EGL/ANGLE initialisation.

Anything that touches Window/Layer has to wait until system/initialize.tjs has
run, which happens after startup.tjs.
