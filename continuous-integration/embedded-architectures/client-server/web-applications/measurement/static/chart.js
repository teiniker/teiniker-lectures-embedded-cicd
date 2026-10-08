// Draws a line chart of the measurements on the <canvas id="chart"> element.
// Plain JavaScript without external libraries, so it works on devices without internet access.
function drawChart(canvas, measurements) {
    const ctx = canvas.getContext('2d');
    const width = canvas.width, height = canvas.height;
    const left = 50, right = 40, top = 20, bottom = 40;
    const plotWidth = width - left - right, plotHeight = height - top - bottom;

    ctx.clearRect(0, 0, width, height);
    ctx.font = '12px sans-serif';
    if (measurements.length === 0) {
        ctx.fillText('Waiting for measurements...', left, top + 20);
        return;
    }

    // Y-axis range: values rounded to whole degrees, at least 1 degree
    const values = measurements.map(m => m.temperature);
    let min = Math.floor(Math.min(...values)), max = Math.ceil(Math.max(...values));
    if (max - min < 1) { max = min + 1; }

    const x = i => left + (measurements.length > 1 ? i * plotWidth / (measurements.length - 1) : 0);
    const y = v => top + plotHeight - (v - min) * plotHeight / (max - min);

    // Horizontal grid lines and y-axis labels
    ctx.strokeStyle = '#ddd';
    ctx.fillStyle = '#333';
    ctx.textAlign = 'right';
    ctx.textBaseline = 'middle';
    for (let v = min; v <= max; v++) {
        ctx.beginPath();
        ctx.moveTo(left, y(v));
        ctx.lineTo(left + plotWidth, y(v));
        ctx.stroke();
        ctx.fillText(v + ' °C', left - 5, y(v));
    }

    // X-axis labels: first, middle and last timestamp
    ctx.textAlign = 'center';
    ctx.textBaseline = 'top';
    const last = measurements.length - 1;
    for (const i of new Set([0, Math.floor(last / 2), last])) {
        ctx.fillText(measurements[i].time, x(i), top + plotHeight + 10);
    }

    // Measurement line
    ctx.strokeStyle = '#c0392b';
    ctx.lineWidth = 2;
    ctx.beginPath();
    measurements.forEach((m, i) => i === 0 ? ctx.moveTo(x(i), y(m.temperature)) : ctx.lineTo(x(i), y(m.temperature)));
    ctx.stroke();
    ctx.lineWidth = 1;
}

// Polls the JSON API periodically and redraws the chart.
function startLiveChart(url, interval) {
    const canvas = document.getElementById('chart');

    async function update() {
        try {
            const response = await fetch(url);
            const measurements = await response.json();
            drawChart(canvas, measurements);
            if (measurements.length > 0) {
                const latest = measurements[measurements.length - 1];
                document.getElementById('value').textContent = latest.temperature.toFixed(1);
                document.getElementById('time').textContent = latest.time;
            }
        } catch (error) {
            console.error('Update failed:', error);
        }
    }

    update();
    setInterval(update, interval);
}
