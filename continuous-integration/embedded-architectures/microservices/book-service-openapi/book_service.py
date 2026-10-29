from http import HTTPStatus
from apiflask import APIFlask, Schema, abort
from apiflask.fields import Integer, String, List, Nested

# Simulate a database table in a list
table = [
    {'id':1, 'author':'Eric Matthes','title':'Python Crash Course', 'isbn':'978-1718502703'},
    {'id':2, 'author':'Brett Slatkin','title':'Effective Python', 'isbn':'978-0134853987'},
    {'id':3, 'author':'Luciano Ramalho','title':'Fluent Python', 'isbn':'978-1492056355'}
]


class BookIn(Schema):
    author = String(required=True)
    title = String(required=True)
    isbn = String(required=True)


class BookCreateIn(BookIn):
    id = Integer(required=True)


class BookOut(Schema):
    id = Integer()
    author = String()
    title = String()
    isbn = String()


class BooksOut(Schema):
    data = List(Nested(BookOut))


app = APIFlask(__name__, title='Book Service', version='1.0.0')

@app.get('/books')
@app.output(BooksOut)
def find_all():
    return {'data': table}


@app.get('/books/<int:oid>')
@app.output(BookOut)
@app.doc(responses=[HTTPStatus.NOT_FOUND])
def find_by_id(oid):
    for item in table:
        print(item)
        if item['id'] == oid:
            return item
    abort(HTTPStatus.NOT_FOUND, 'book not found')


@app.post('/books')
@app.input(BookCreateIn)
@app.output(BookOut, status_code=HTTPStatus.CREATED)
def insert(json_data):
    print(json_data)
    book = {
        'id': json_data['id'],
        'author': json_data['author'],
        'title': json_data['title'],
        'isbn': json_data['isbn']
    }
    table.append(book)
    return book


@app.put('/books/<int:oid>')
@app.input(BookIn)
@app.output(BookOut)
@app.doc(responses=[HTTPStatus.NOT_FOUND])
def update(oid, json_data):
    for item in table:
        print(item)
        if item['id'] == oid:
            print(json_data)
            item.update({'author':json_data['author'], 'title':json_data['title'], 'isbn':json_data['isbn']})
            return item
    abort(HTTPStatus.NOT_FOUND, 'book not found')


@app.delete('/books/<int:oid>')
@app.output({}, status_code=HTTPStatus.NO_CONTENT)
@app.doc(responses=[HTTPStatus.NOT_FOUND])
def delete(oid):
    for item in table:
        print(item)
        if item['id'] == oid:
            table.remove(item)
            return ''
    abort(HTTPStatus.NOT_FOUND, 'book not found')


if __name__ == '__main__':
    app.run(port=8080, debug=True)
