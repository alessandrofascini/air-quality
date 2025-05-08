const fs = require("node:fs");
const express = require("express");
const app = express();

app.use(express.text());

app.post("/", (req, res) => {
  req.accepts("text/plain");

  const timestamp = new Date();
  const [temperature, humidity, ...R] = req.body.split(";").map((v) => +v);

  console.log(
    `[${timestamp.toISOString()}] T = ${temperature.toFixed(
      1
    )} °C ± 2; RH = ${humidity} % ± 5; R0 = {${R}} KΩ`
  );
  res.send();

  const content = timestamp.toISOString() + ";" + req.body + "\n";
  fs.appendFile(
    "measurements.txt",
    content,
    (err) => err && console.error(err)
  );
});

const port = 5173;
app.listen(port, () => {
  console.log(`App is running on port: ${port}`);
});
