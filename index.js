const clientId = 'mqttjs_' + Math.random().toString(16).substr(2, 8);
const host = 'ws://broker.emqx.io:8083/mqtt';
const topic_ldr = 'm1n4mqtt/92917926129121/ldr';
const topic_led = 'm1n4mqtt/92917926129121/led';

//Pengaturan MQTT----------------------------------------------------
const options = {
    keepalive: 60,
    clientId: clientId,
    protocolId: 'MQTT',
    protocolVersion: 4,
    clean: true,
    reconnectPeriod: 1000,
    connectTimeout: 30 * 1000,
    will: {
        topic: 'WillMsg',
        payload: 'Connection Closed abnormally..!',
        qos: 0,
        retain: false
    },
};

//Koneksi MQTT-------------------------------------------------------
console.log('Connecting mqtt client');
const client = mqtt.connect(host, options);
client.on('error', (err) => {
    console.log('Connection error: ', err);
    client.end();
});
client.on('reconnect', () => {
    console.log('Reconnecting...');
});

//Ketika terkoneksi--------------------------------------------------
client.on('connect', () => {
    console.log(`Client connected: ${clientId}`)
    // Subscribe
    client.subscribe(topic_ldr, { qos: 0 })
})


function led_on() {
    client.publish(topic_led, 'ON', { qos: 0, retain: false })
}

function led_off() {
    client.publish(topic_led, 'OFF', { qos: 0, retain: false })
}

//Ketika menerima pesan----------------------------------------------   
client.on('message', (topic, message, packet) => {
    console.log(`Received Message: ${message.toString()} On topic: ${topic}`)

    //Menampilkan Nilai LDR ke HTML
    if (topic == topic_ldr) {
        document.getElementById("ldr").innerHTML = message.toString();
    }

})