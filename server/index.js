const express = require('express')
const app = express()
const port = 3000

var num = 1;

app.get('/', (req, res) => {
  console.log("Got a request")
  res.send(`bruh ${num}`)
  num += 1
})

app.get('/ping', (req, res) => {
  console.log("Got a request")
  res.send(`pong ${num}`)
  num += 1
})

app.listen(port, () => {
  console.log(`Example app listening on port ${port}`)
})