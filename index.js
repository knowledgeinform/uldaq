const uldaq = require('bindings')('uldaq')
const process = require('process')
process.on('exit', (code) => {
    uldaq.closeAll() // avoids segmentation faults
})

module.exports = uldaq
