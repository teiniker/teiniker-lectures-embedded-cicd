# Example: Translator

## Setup 

We start the web application from the command line:
```
$ python app.py
```

Using a Browser, we can access the web page at:
```
http://localhost:8080/
```


## Flask Implementation 

The following example demonstrates how to implement the MVC pattern in a Flask application:

* **Initialize the App**

```python
app = Flask(__name__)
```

- This creates an instance of the Flask app.
- This instance is needed to define routes and run the application.


* **Route for Homepage**

```python
@app.route('/')
def index():
    return render_template('index.html')
```

- Defines the root URL /.
- When a user visits this page, the app will render `index.html`.
- Typically, this page contains a form that asks the user to input 
    a word and select a language.

* **Route to Handle Translation**

```python
@app.route('/translator', methods=['POST'])
def translate():
    language = request.form.get('language')
    word = request.form.get('word')

    if language == "Deutsch":
        service = TranslatorServiceGerman()
    else:
        service = TranslatorServiceFrench()

    translation = service.translate(word)

    return render_template('translation.html', word=word, translation=translation)
```

- This route handles form submissions sent via `POST` method to `/translator`.

- `request.form.get(...)` retrieves user input from the form.
    - `language`: The selected language (`Deutsch` or anything else 
        assumed to be `French`).
    - `word`: The word the user wants translated.

- Based on the selected language, the appropriate translation service is instantiated.

- These services implement a method `.translate(word)` that returns the translated word.

- The app then renders `translation.html`, passing the original word 
    and its translation as template variables.
    The template will display the translation result to the user.


* **Run the Webb Application**

```python
if __name__ == '__main__':
    app.run(port=8080, debug=True)
```

- Starts the development server on port `8080`.

- `debug=True` enables hot reloading and detailed error messages for easier development.


* **HTML Templates**
The example uses a Jinja2 HTML template to display the translation result.

```html
<h2>
    Translate: {{ word }} into {{ translation }}
</h2>

- `{{ word }}` and `{{ translation }}` are Jinja2 placeholders.

- Flask passes values for these when rendering the template via `render_template(...)`.

```html
<a href="{{ url_for('index') }}">Back</a>
```

- This is a link that lets the user return to the home page.
- `{{ url_for('index') }}` generates the correct URL for the Flask route 
    named `'index'`.


*Egon Teiniker, 2025-2026, GPL v3.0*