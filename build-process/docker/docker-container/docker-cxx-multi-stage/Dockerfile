# ---------- Build Stage ----------

FROM alpine:latest AS build

RUN apk add --no-cache g++ make cmake

WORKDIR /app
COPY . .

RUN mkdir -p build && cd build && cmake .. && make

# ---------- Runtime Stage ----------

FROM alpine:latest

WORKDIR /app
COPY --from=build /app/build/hello .

CMD ["./hello"]



