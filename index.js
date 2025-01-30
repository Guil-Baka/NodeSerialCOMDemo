import { SerialPort } from "serialport";
import { ReadlineParser } from "@serialport/parser-readline";
import path from "path";

const myPort = new SerialPort({
  path: "/dev/ttyACM0",
  // path: "COM12",
  baudRate: 9600,
  autoOpen: true,
});

console.log("Serial port is open");
// get the data from the serial port 
const parser = myPort.pipe(new ReadlineParser({ delimiter: "\n" }));
parser.on("data", (data) => {
  console.log(`${data}`);
  if (data.includes("EOL")) {
    myPort.close();
    console.log("Serial port is closed");
  }
});


// let i = 10;
// while (i > -1) {
//   myPort.write(`<${i}>`);
//   console.log(`Sent: <${i}>`);
//   // delay 200ms
//   await new Promise((resolve) => setTimeout(resolve, 1000));

//   i--;
// }

// now send data using "," as delimiter
// myPort.write("Hello,World\n");

for (let i = 0; i < 10; i++) {
  myPort.write(`Hello,World,pinto\n`);
  // console.log(`Sent: <${i}>`);
}


