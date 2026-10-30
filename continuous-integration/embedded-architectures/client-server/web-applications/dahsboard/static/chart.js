// Draws a line chart of one field of the measurements on a <canvas> element.
// options: { field: 'temperature', unit: '°C', color: '#c0392b' }
// Plain JavaScript without external libraries, so it works on devices without internet access.
function drawChart(canvas, measurements, options) {
    const ctx = canvas.getContext('2d');
    const width = canvas.width, height = canvas.height;
    const left = 60, right = 40, top = 20, bottom = 40;
    const plotWidth = width - left - right, plotHeight = height - top - bottom;

    ctx.clearRect(0, 0, width, height);
    ctx.font = '12px sans-serif';
    ctx.fillStyle = '#333';
    ctx.textAlign = 'left';
    ctx.textBaseline = 'alphabetic';
    if (measurements.length === 0) {
        ctx.fillText('Waiting for measurements...', left, top + 20);
        return;
    }

    // Y-axis range: grid step of 1, 2, 5 or 10 units, so that about 5 grid lines are drawn
    const values = measurements.map(m => m[options.field]);
    const span = Math.max(Math.max(...values) - Math.min(...values), 1);
    const step = [1, 2, 5, 10].find(s => span / s <= 5) || 20;
    const min = Math.floor(Math.min(...values) / step) * step;
    let max = Math.ceil(Math.max(...values) / step) * step;
    if (max === min) { max = min + step; }

    const x = i => left + (measurements.length > 1 ? i * plotWidth / (measurements.length - 1) : 0);
    const y = v => top + plotHeight - (v - min) * plotHeight / (max - min);

    // Horizontal grid lines and y-axis labels
    ctx.strokeStyle = '#ddd';
    ctx.textAlign = 'right';
    ctx.textBaseline = 'middle';
    for (let v = min; v <= max; v += step) {
        ctx.beginPath();
        ctx.moveTo(left, y(v));
        ctx.lineTo(left + plotWidth, y(v));
        ctx.stroke();
        ctx.fillText(v + ' ' + options.unit, left - 5, y(v));
    }

    // X-axis labels: first, middle and last timestamp
    ctx.textAlign = 'center';
    ctx.textBaseline = 'top';
    const last = measurements.length - 1;
    for (const i of new Set([0, Math.floor(last / 2), last])) {
        ctx.fillText(measurements[i].time, x(i), top + plotHeight + 10);
    }

    // Measurement line
    ctx.strokeStyle = options.color;
    ctx.lineWidth = 2;
    ctx.beginPath();
    measurements.forEach((m, i) => i === 0 ? ctx.moveTo(x(i), y(m[options.field])) : ctx.lineTo(x(i), y(m[options.field])));
    ctx.stroke();
    ctx.lineWidth = 1;
}
