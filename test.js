const uldaq = require('bindings')('uldaq')
const process = require('process')
process.on('exit', (code) => {
    uldaq.closeAll() // avoids segmentation faults
})

console.log(uldaq)
var aiConfig = uldaq.config.AiConfigItem
console.log(aiConfig)

var devices = uldaq.getDaqDeviceInventory()
// console.log(devices)
devices.forEach((element, i) => {
    console.log(i, element)
    console.log('Has analog',uldaq.devInfoHasAi(i))
    console.log('Has temperature',uldaq.aiInfoHasTempChan(i))
});

console.log(uldaq.connectDaqDevice(0))
console.log(uldaq.aiSetConfig(0, aiConfig.AI_CFG_CHAN_TC_TYPE.value, 0, aiConfig.AI_CFG_CHAN_TC_TYPE.options.TC_K))
// console.log(uldaq.devInfoHasAi(0))

// async function f() {

// }

setInterval(() => {
    console.log(uldaq.TIn(0))
}, 1000)
