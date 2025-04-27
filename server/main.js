const express = require("express");
const app = express();

app.use(express.text());

app.post("/", (req, res) => {
  req.accepts("text/plain");

  const [temperature, humidity] = req.body
    .split(";")
    .map((v) => (+v).toFixed(2));
  console.log(
    `[${new Date().toISOString()}] Temperature ${temperature} °C ± 2; Humidity ${humidity} % ± 5`
  );

  res.send();
});

const port = 5173;
app.listen(port, () => {
  console.log(`App is running on port: ${port}`);
});
