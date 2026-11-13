# Failing script followed by a valid one

Mirrors the demo's `startup.tjs`: `execStorage(bad)` then `execStorage(next)`.
If the engine is left in a broken state by the first failure, the second call is
where it shows up.
