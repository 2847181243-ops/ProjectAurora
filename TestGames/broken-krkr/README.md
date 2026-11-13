# Broken KiriKiri game

`startup.tjs` contains `@if`, which the TJS parser rejects.
If the app dies on this fixture, a bad game can crash the host.
