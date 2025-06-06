const express = require("express");
const app = express();
const { MongoClient } = require("mongodb");

require("dotenv").config();

const {
  MONGODB_USERNAME,
  MONGODB_PASSWORD,
  MONGODB_HOST,
  MONGODB_PORT,
  MONGODB_AUTH_SOURCE,
  MONGODB_DATABASE,
  MONGODB_COLLECTION,
} = process.env;

app.use(express.text());

const client = (() => {
  const uri = `mongodb://${MONGODB_USERNAME}:${MONGODB_PASSWORD}@${MONGODB_HOST}:${MONGODB_PORT}/${MONGODB_DATABASE}?authSource=${MONGODB_AUTH_SOURCE}`;
  return new MongoClient(uri);
})();

async function connect() {
  try {
    await client.connect();
    return [
      client,
      undefined,
      async () => {
        try {
          await client.close();
        } catch (reason) {
          console.error("an error occurs during collection closing");
        }
      },
    ];
  } catch (reason) {
    return [undefined, reason, () => {}];
  }
}

function getCollection(client) {
  try {
    const db = client.db();
    return [db.collection(MONGODB_COLLECTION), undefined];
  } catch (reason) {
    return [undefined, reason];
  }
}

app.post("/", async (req, res) => {
  req.accepts("text/plain");

  // get database
  const [client, clientError, closeConnection] = await connect();
  if (clientError) {
    console.error(dbError);
    res.status(503);
    res.send();
    return;
  }

  // get collection
  const [collection, collectionError] = await getCollection(client);
  if (collectionError) {
    await closeConnection();
    console.error(dbError);
    res.status(500);
    res.send();
    return;
  }

  try {
    const timestamp = new Date();

    const [device_id, temperature, humidity, raw_value, iaqi] = req.body
      .split(";")
      .filter((v) => v !== "")
      .map((v) => {
        const n = +v;
        if (Number.isNaN(n)) {
          return v;
        }
        return n;
      });

    console.log(
      `[${timestamp.toISOString()}] ${device_id}: T = ${temperature.toFixed(
        1
      )} °C ± 2; RH = ${humidity} % ± 5; raw_value = ${raw_value}; IAQI = ${iaqi}`
    );

    await collection.insertOne({
      device_id,
      timestamp,
      temperature,
      humidity,
      raw_value,
      iaqi,
    });
    console.log(`[${new Date().toISOString()}] new measure inserted!`);
    res.status(200);
  } catch (reason) {
    console.error(reason);
    res.status(500);
  } finally {
    res.send();
    await closeConnection();
  }
});

const port = 5173;
app.listen(port, () => {
  console.log(`App is running on port: ${port}`);
});
