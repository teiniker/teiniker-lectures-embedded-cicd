const TEMPERATURE = { field: 'temperature', unit: '°C', color: '#c0392b' };
const HUMIDITY = { field: 'humidity', unit: '%RH', color: '#2980b9' };

// Polls the JSON API periodically and redraws both charts.
function startDashboard(url, interval) {
    const temperatureCanvas = document.getElementById('temperature-chart');
    const humidityCanvas = document.getElementById('humidity-chart');

    async function update() {
        try {
            const response = await fetch(url);
            if (!response.ok) throw new Error('HTTP ' + response.status);
            const measurements = await response.json();
            drawChart(temperatureCanvas, measurements, TEMPERATURE);
            drawChart(humidityCanvas, measurements, HUMIDITY);
        } catch (error) {
            // Keep the last drawn charts; the next interval tries again
            console.error('Update failed:', error);
        }
    }

    update();
    setInterval(update, interval);
}
