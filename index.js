import { SerialPort } from "serialport";
import { ReadlineParser } from "@serialport/parser-readline";
import path from "path";

const myPort = new SerialPort({
  path: "/dev/ttyACM0",
  baudRate: 9600,
  autoOpen: true,
});

console.log("Serial port is open");

const parser = new ReadlineParser();
myPort.pipe(parser);
parser.on("data", (line) => { console.log(line); });

let i = 10;
while (i > -1) {
  myPort.write(`<${i}>`);
  console.log(`Sent: <${i}>`);
  i--;
}
