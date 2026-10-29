from http import HTTPStatus
from apiflask import APIBlueprint, APIFlask, Schema, abort
from apiflask.fields import Integer, String, List, Nested
from message import Message
from message_dao import MessageDao


class MessageIn(Schema):
    priority = Integer(required=True)
    data = String(required=True)


class MessageCreateIn(MessageIn):
    address = Integer(required=True)


class MessageOut(Schema):
    address = Integer()
    priority = Integer()
    data = String()


class MessagesOut(Schema):
    data = List(Nested(MessageOut))


dao = MessageDao()

# All routes of version 1 of the REST API are prefixed with /v1
api_version = APIBlueprint('api_v1', __name__, url_prefix='/v1')


@api_version.get('/messages')
@api_version.output(MessagesOut)
def find_all():
    return {'data': dao.find_all()}


@api_version.get('/messages/<int:address>')
@api_version.output(MessageOut)
@api_version.doc(responses=[HTTPStatus.NOT_FOUND])
def find_by_id(address):
    msg = dao.find_by_id(address)
    if msg is None:
        abort(HTTPStatus.NOT_FOUND, 'message not found')
    return msg


@api_version.post('/messages')
@api_version.input(MessageCreateIn)
@api_version.output(MessageOut, status_code=HTTPStatus.CREATED)
def insert(json_data):
    dto = Message(
        address=json_data['address'],
        priority=json_data['priority'],
        data=json_data['data']
    )
    return dao.insert(dto)


@api_version.put('/messages/<int:address>')
@api_version.input(MessageIn)
@api_version.output(MessageOut)
@api_version.doc(responses=[HTTPStatus.NOT_FOUND])
def update(address, json_data):
    dto = Message(
        address=address,
        priority=json_data['priority'],
        data=json_data['data']
    )
    updated = dao.update(address, dto)
    if updated is None:
        abort(HTTPStatus.NOT_FOUND, 'message not found')
    return updated


@api_version.delete('/messages/<int:address>')
@api_version.output({}, status_code=HTTPStatus.NO_CONTENT)
@api_version.doc(responses=[HTTPStatus.NOT_FOUND])
def delete(address):
    if not dao.delete(address):
        abort(HTTPStatus.NOT_FOUND, 'message not found')
    return ''


app = APIFlask(__name__, title='Message Service', version='1.0.0')
app.register_blueprint(api_version)

if __name__ == '__main__':
    app.run(port=8080, debug=True)
