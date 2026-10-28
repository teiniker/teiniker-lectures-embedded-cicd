# Hypertext Markup Language 

## Introduction

> The **Hypertext Markup Language (HTML)** is the standard markup language
> for web pages. It describes the **structure and content** of a document
> using **elements** that are written as **tags**.

A web browser loads an HTML document (from a file or via HTTP), parses it
into a tree of elements, the **Document Object Model (DOM)**, and renders
it on the screen. HTML is usually combined with two other technologies:

* **HTML**: Structure and content (headings, paragraphs, tables, forms, ...).
* **CSS** (Cascading Style Sheets): Presentation (colors, fonts, layout).
* **JavaScript**: Behavior (reacting to user input, loading data, ...).

In a web application, the server generates HTML pages (e.g. using Jinja2
templates in Flask) and sends them in the body of an HTTP response with the
header field `Content-Type: text/html`.


### Elements, Tags and Attributes

An **element** usually consists of a **start tag**, **content**, and an
**end tag**. The start tag can contain **attributes** (name-value pairs)
which provide additional information about the element.

```html
<a href="https://www.w3schools.com/html/">HTML Tutorial</a>
```

* `<a ...>`: Start tag of an anchor (link) element.
* `href="..."`: Attribute which defines the link target.
* `HTML Tutorial`: Content, displayed in the browser.
* `</a>`: End tag.

Some elements have no content and therefore no end tag
(**void elements**), e.g. `<br>`, `<img>`, `<input>`, or `<meta>`.

Elements can be **nested**, which results in a tree structure (DOM).
HTML is not case-sensitive, but lowercase tags are common practice.

Two attributes can be used with all elements:
* `id`: A **unique** name of an element within the page (used by CSS and
  JavaScript to find a specific element).
* `class`: One or more class names, used to style groups of elements.


### Basic Page Structure

Every HTML page follows the same basic structure:

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Temperature Measurement</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <h1>Temperature Measurement</h1>
    <p>Current Temperature: <span id="value">21.5</span> &deg;C</p>
</body>
</html>
```

* `<!DOCTYPE html>`: Tells the browser that the document uses HTML5.
  Without it, browsers render the page in a backward-compatible
  **quirks mode**.

* `<html>`: Root element of the page. The `lang` attribute defines the
  language of the content.

* `<head>`: Metadata about the page, which is **not displayed**:
    - `<meta charset="UTF-8">`: Character encoding of the document.
    - `<title>`: Title shown in the browser tab and in bookmarks.
    - `<link>`: Includes external resources such as CSS files.
    - `<style>`: Embedded CSS rules.
    - `<script>`: JavaScript code (can also be placed in the `<body>`).

* `<body>`: The **visible content** of the page.

Special characters must be written as **character entities**, e.g.
`&lt;` for `<`, `&gt;` for `>`, `&amp;` for `&`, or `&deg;` for `°`.
Comments are written as `<!-- comment -->`.


### Basic Page Elements

The following elements are used in most HTML pages:

| Element                      | Description                               |
|------------------------------|-------------------------------------------|
| `<h1>` ... `<h6>`            | Headings (level 1 to 6)                   |
| `<p>`                        | Paragraph                                 |
| `<br>`                       | Line break                                |
| `<hr>`                       | Horizontal line (thematic break)          |
| `<b>`, `<strong>`            | Bold text, important text                 |
| `<i>`, `<em>`                | Italic text, emphasized text              |
| `<a href="...">`             | Hyperlink to another page or resource     |
| `<img src="..." alt="...">`  | Image (`alt`: text if image is missing)   |
| `<ul>`, `<ol>`, `<li>`       | Unordered list, ordered list, list item   |
| `<table>`, `<tr>`, `<th>`, `<td>` | Table, row, header cell, data cell   |
| `<div>`                      | Block container (groups elements)         |
| `<span>`                     | Inline container (part of a text)         |
| `<form>`, `<input>`, ...     | Forms for user input (see below)          |
| `<canvas>`                   | Drawing area for JavaScript graphics      |

Elements are either displayed as:
* **Block elements** (e.g. `<h1>`, `<p>`, `<div>`, `<table>`): Start on a
  new line and use the full available width.
* **Inline elements** (e.g. `<a>`, `<span>`, `<b>`, `<img>`): Are placed
  within the text flow and use only the necessary width.

_Example_: Lists and tables

```html
<h2>Sensors</h2>
<ul>
    <li>Temperature</li>
    <li>Humidity</li>
</ul>

<table>
    <tr> <th>Time</th>     <th>Temperature</th> </tr>
    <tr> <td>10:00:00</td> <td>21.5</td>        </tr>
    <tr> <td>10:00:01</td> <td>21.6</td>        </tr>
</table>
```

HTML5 also provides **semantic elements** such as `<header>`, `<nav>`,
`<main>`, `<section>`, `<article>`, and `<footer>`. They behave like
`<div>`, but describe the meaning of a page region, which helps screen
readers and search engines.


## HTML Forms

> An **HTML form** collects user input in the browser and sends it to the
> server as part of an **HTTP request**, where it is processed by a
> Controller (e.g. a Flask route handler).

Forms are the classic way for users to send data to a web application,
e.g. to change the configuration of an embedded device (IP address,
sampling interval, threshold values, ...).


### Form Elements

A form is defined by the `<form>` element, which contains input elements:

```html
<form method="POST" action="/translator">
    <label for="word">Word:</label>
    <input type="text" id="word" name="word" maxlength="30" required>

    <label for="language">Language:</label>
    <select id="language" name="language">
        <option value="de">Deutsch</option>
        <option value="fr">Francais</option>
    </select>

    <input type="reset"  value="Reset">
    <input type="submit" value="Translate">
</form>
```

* `<form>` attributes:
    - `action`: URL to which the form data is sent (default: the URL of the
      current page).
    - `method`: HTTP method, either `GET` (default) or `POST`.

* `<input>`: The most important input element, its `type` attribute
  defines how it is displayed:

| Type                     | Description                                   |
|--------------------------|-----------------------------------------------|
| `text`                   | Single-line text field                        |
| `password`               | Text field with hidden characters             |
| `number`                 | Numeric value (`min`, `max`, `step`)          |
| `email`, `url`, `date`   | Text fields with format validation            |
| `checkbox`               | On/off option (multiple selections possible)  |
| `radio`                  | One option out of a group (same `name`)       |
| `hidden`                 | Value not visible to the user                 |
| `submit`                 | Button which sends the form                   |
| `reset`                  | Button which resets all fields                |

* Other input elements:
    - `<select>` with `<option>`: Drop-down list.
    - `<textarea>`: Multi-line text field.
    - `<button>`: Button with arbitrary content.
    - `<label>`: Caption of an input element (the `for` attribute refers
      to the `id` of the element).

* The **`name` attribute** is essential: only elements with a `name` are
  sent to the server, and the name is used as the **parameter name**.
  The `id` attribute is only used within the page (CSS, JavaScript, labels).

* Attributes such as `required`, `maxlength`, `min`, `max`, or `pattern`
  enable **client-side validation** in the browser.


### Form Processing

When the user clicks the submit button, the browser processes the form
as follows:

1. The browser collects all input elements with a `name` attribute and
   their current values.

2. The values are encoded as **name-value pairs**
   (`application/x-www-form-urlencoded`). Special characters are
   percent-encoded, spaces are replaced by `+`:

```
word=Hello+World&language=de
```

3. The browser sends an HTTP request to the URL given in `action`:

    - **`GET`**: The data is appended to the URL as a **query string**.
      It is visible in the address bar, in the browser history, and in
      server logs, and can be bookmarked. Use `GET` for requests which
      only **read data** (e.g. a search).

    ```
    GET /translator?word=Hello+World&language=de HTTP/1.1
    Host: localhost:8080
    ```

    - **`POST`**: The data is sent in the **request body**. Use `POST` for
      requests which **change data** on the server (e.g. a configuration
      change) or contain sensitive data.

    ```
    POST /translator HTTP/1.1
    Host: localhost:8080
    Content-Type: application/x-www-form-urlencoded
    Content-Length: 28

    word=Hello+World&language=de
    ```

4. On the server, the web framework decodes the parameters and passes them
   to the Controller. In Flask, query parameters are available via
   `request.args` and form data via `request.form`:

```python
@app.route('/translator', methods=['POST'])
def translate():
    word = request.form.get('word')
    language = request.form.get('language')
    ...
    return render_template('translation.html', word=word, translation=translation)
```

5. The Controller returns a new HTML page, which replaces the current page
   in the browser (**full page reload**).


## JavaScript 

> **JavaScript** is a programming language which is executed by the
> **web browser**. It makes HTML pages **interactive and dynamic**
> without loading a new page from the server.

While HTML defines the structure and CSS the presentation, JavaScript
defines the **behavior** of a page. Typical tasks are:

* **Reacting to user events**: Clicks, key presses, form input, ...
* **Manipulating the DOM**: Changing the content, attributes, or style of
  elements after the page has been loaded.
* **Validating input** before a form is sent.
* **Communicating with the server** in the background (HTTP requests via
  the `fetch()` API), e.g. to load JSON data from a REST API.
* **Drawing graphics** on a `<canvas>` element (e.g. live charts).


### Including JavaScript

JavaScript code is included using the `<script>` element, either embedded
in the page or loaded from an external file:

```html
<!-- external file, in Flask served from the static/ directory -->
<script src="{{ url_for('static', filename='chart.js') }}"></script>

<!-- embedded code -->
<script>
    startLiveChart("{{ url_for('measurements') }}", 1000);
</script>
```

Scripts are executed while the page is parsed, so elements accessed by
a script must be defined **before** the `<script>` element (or the
`defer` attribute is used).


### DOM Manipulation and Events

JavaScript accesses the elements of a page via the `document` object:

```html
<p>Current Temperature: <span id="value">--</span> &deg;C</p>
<button id="reset">Reset</button>

<script>
    const value = document.getElementById('value');
    const button = document.getElementById('reset');

    button.addEventListener('click', () => {
        value.textContent = '--';
    });
</script>
```

* `document.getElementById()`, `document.querySelector()`: Find elements.
* `element.textContent`: Read or change the text of an element
  (`innerHTML` interprets the value as HTML and can lead to **XSS**
  vulnerabilities if it contains user input).
* `element.addEventListener()`: Register a function which is called when
  an event (`click`, `input`, `submit`, ...) occurs.


### Loading Data from the Server

With the `fetch()` API, JavaScript can send HTTP requests in the background
and update only parts of the page. This technique is known as
**AJAX** (Asynchronous JavaScript and XML), although today JSON is used
instead of XML.

```javascript
async function update() {
    const response = await fetch('/api/temperature');
    if (response.ok) {
        const measurement = await response.json();
        document.getElementById('value').textContent = measurement.temperature;
    }
}

setInterval(update, 1000);      // poll the server every second
```

Compared to HTML forms, this approach has the following properties:

| HTML Forms                          | JavaScript (`fetch()`)               |
|-------------------------------------|--------------------------------------|
| Server returns a complete HTML page | Server returns data (JSON)           |
| Full page reload                    | Only parts of the page are updated   |
| Works without JavaScript            | Requires JavaScript in the browser   |
| Logic mainly on the server          | Part of the logic runs in the browser|

For embedded devices, this approach is well suited to display **live
data** (sensor values, status information): the device provides a small
REST API, and the browser does the rendering work. A complete example is
the [Measurement](../measurement/README.md) web application, which draws
a live chart of temperature values on a `<canvas>` element.

Note that embedded devices are often operated **without internet access**.
Therefore, JavaScript libraries (e.g. chart libraries) should be served by
the device itself from the `static/` directory instead of being loaded
from a CDN.


## CSS

> **Cascading Style Sheets (CSS)** is a language which describes the
> **presentation** of HTML elements: colors, fonts, spacing, and layout.

The idea of CSS is the **separation of content and presentation**: HTML
defines what is shown, CSS defines how it is shown. The same HTML page can
be displayed differently (e.g. on a desktop and on a smartphone) by
changing only the CSS rules, and one CSS file can define a common look for
all pages of a web application.


### CSS Rules

A style sheet consists of **rules**. Each rule has a **selector**, which
defines the elements the rule applies to, and a **declaration block** with
**property-value pairs**:

```css
h2 {
    color: #2c3e50;
    font-size: 1.5em;
}
```

* `h2`: Selector, the rule applies to all `<h2>` elements.
* `color`, `font-size`: Properties.
* `#2c3e50`, `1.5em`: Values.

The most important selectors are:

| Selector         | Example             | Selects                           |
|------------------|---------------------|-----------------------------------|
| Element          | `p`                 | All `<p>` elements                |
| Class            | `.warning`          | All elements with `class="warning"` |
| ID               | `#value`            | The element with `id="value"`     |
| Descendant       | `table td`          | `<td>` elements inside a `<table>` |
| Group            | `h1, h2`            | All `<h1>` and `<h2>` elements    |
| Pseudo-class     | `a:hover`           | Links under the mouse pointer     |


### Including CSS

CSS rules can be added to an HTML page in three ways:

* **External style sheet**: A separate `.css` file, included with the
  `<link>` element in the `<head>`. This is the preferred way, because
  the file can be shared by all pages and cached by the browser.

```html
<!-- in Flask served from the static/ directory -->
<link rel="stylesheet" href="{{ url_for('static', filename='style.css') }}">
```

* **Embedded style sheet**: Rules within a `<style>` element in the
  `<head>`, valid for this page only.

```html
<style>
    body   { font-family: sans-serif; margin: 2em; }
    #value { font-weight: bold; color: #c0392b; }
</style>
```

* **Inline style**: The `style` attribute of a single element. This mixes
  content and presentation and should be avoided.

```html
<p style="color: red;">Sensor not connected!</p>
```


### Cascade and Inheritance

Several rules can apply to the same element. The **cascade** decides
which declaration wins:

1. **Specificity**: A more specific selector wins
   (inline style > ID > class > element).
2. **Order**: For equal specificity, the rule defined last wins.

In addition, some properties (e.g. `color`, `font-family`) are
**inherited** by nested elements. A font defined for `<body>` is therefore
used for the whole page, unless a more specific rule overrides it.


### Box Model and Layout

The browser renders each element as a rectangular **box**, which consists
of (from inside to outside):

* **Content**: Text or images, size defined by `width` and `height`.
* **Padding**: Space between the content and the border.
* **Border**: A line around the padding.
* **Margin**: Space between the border and neighboring elements.

```css
.card {
    padding: 1em;
    border: 1px solid #ccc;
    margin: 1em 0;
}
```

For the arrangement of elements on a page, CSS provides layout models
such as **Flexbox** (`display: flex`, one-dimensional rows or columns) and
**Grid** (`display: grid`, two-dimensional). With **media queries**,
rules can be applied depending on the screen size, which allows
**responsive** pages that work on desktops and smartphones:

```css
@media (max-width: 600px) {
    body { margin: 0.5em; }
}
```

For embedded devices, a small hand-written style sheet is often sufficient.
CSS frameworks (e.g. Bootstrap) can be used as well, but should be served
from the device's `static/` directory if there is no internet access.


## References

* [HTML Tutorial](https://www.w3schools.com/html/)
* [MDN Web Docs: HTML](https://developer.mozilla.org/en-US/docs/Web/HTML)
* [MDN Web Docs: Web forms](https://developer.mozilla.org/en-US/docs/Learn_web_development/Extensions/Forms)
* [MDN Web Docs: JavaScript](https://developer.mozilla.org/en-US/docs/Web/JavaScript)
* [MDN Web Docs: CSS: Cascading Style Sheets](https://developer.mozilla.org/en-US/docs/Web/CSS)

_Egon Teiniker, 2025-2026, GPL v3.0_
