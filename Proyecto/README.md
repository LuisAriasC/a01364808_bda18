# Restaurant project with Neo4j, docker compose and Node.js project

## Cesar Valladares
## Luis Carlos Arias

install the docker cotainers and run application just run in terminal: 
- `$ make`

## dependencies:
- `make`
- `docker` [https://www.docker.com/products/docker-engine](https://www.docker.com/products/docker-engine)
- `docker-compose` [https://docs.docker.com/compose/](https://docs.docker.com/compose/)

## docker.host
It depends on platform:
- OSX: please check `docker-machine ip`
- Linux: `127.0.0.1` or `localhost`

## neo4j
Binds port  17474 [localhost:17474/]
- Login: neo4j
- Password: test

## app
Binds to port 13000  [localhost:13000/]

## Files
- docker-compose.yml
- Makefile
- app.js
- index.html
