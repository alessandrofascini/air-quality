const fs = require("node:fs/promises");

async function analyze() {
  const data = await fs.readFile("measurements.txt", "utf-8");
  return data
    .split("\n")
    .map((row) => {
      return row.split(";");
    })
    .map((row) => {
      const [timestamp, temperature, humidity, ...measurements] = row;
      return {
        timestamp,
        temperature: +temperature - 1,
        humidity: +humidity,
        measurements: measurements.map((v) => +v),
      };
    });
}

analyze().then().then(console.log);
