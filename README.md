# Sync Epoll Client

## Intro
The client side code of a simple `relay server` using `sync epoll`.

## Supported command

1. `start {nSession}`
2. `test {nContent}`
3. `stop`
4. `exit`

## TODO

1. use the average time delay of all sessions as the final result.

## Code Review

1. Bad error handler
> mix  up `perror` and `fprintf(stderr)`
2. Inconsistent naming
> i.e `do_event` and `doWrite`
3. Messy epoll event management.
4. Confusing code logic.