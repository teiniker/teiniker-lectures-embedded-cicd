from datetime import timedelta
from http import HTTPStatus
from flask import Flask, jsonify, request
from flask_jwt_extended  import JWTManager, create_access_token, jwt_required, get_jwt_identity

# Simulate a database table in a list
table = [
    {'id':1, 'description':'Effective Python','price':2390},
    {'id':2, 'description':'Effective Python','price':2390},
    {'id':3, 'description':'Effective Python','price':2390}
]

app = Flask(__name__)

users = {
    "student": "student",
    "homer": "duffbeer"
}

# generate a secret key for JWT
app.config["JWT_SECRET_KEY"] =app.config["JWT_SECRET_KEY"] = "f7b9c6d2a84e4e0bb3c9f5a6e2d14c8a9f1e7d0c4b6a8e53c2f9d71a4e6b0cd"
app.config["JWT_ACCESS_TOKEN_EXPIRES"] = timedelta(minutes=15)

jwt = JWTManager(app)


@app.route("/login", methods=["POST"])
def login():
    data = request.get_json()

    if not data or "username" not in data or "password" not in data:
        return jsonify({"msg": "Missing username or password"}), 400

    username = data["username"]
    password = data["password"]

    if users.get(username) != password:
        return jsonify({"msg": "Bad username or password"}), 401

    # Create JWT
    access_token = create_access_token(identity=username)

    return jsonify(access_token=access_token), 200


@app.route('/articles', methods = ['GET'])
@jwt_required()
def find_all():
    return jsonify({'data': table}), HTTPStatus.OK


@app.route('/articles/<int:oid>', methods = ['GET'])
@jwt_required()
def find_by_id(oid):
    current_user = get_jwt_identity()
    print(f'Current user: {current_user}')

    for item in table:
        print(item)
        if item['id'] == oid:
            return jsonify(item), HTTPStatus.OK
    return jsonify({'message': 'article not found'}), HTTPStatus.NOT_FOUND


@app.route('/articles', methods=['POST'])
@jwt_required()
def insert():
    data = request.get_json()
    print(data)
    oid = data.get('id')
    description = data.get('description')
    price = data.get('price')
    article = {
        'id': oid,
        'description': description,
        'price': price
    }
    table.append(article)
    return jsonify(article), HTTPStatus.CREATED


@app.route('/articles/<int:oid>', methods=['PUT'])
@jwt_required()
def update(oid):
    for item in table:
        print(item)
        if item['id'] == oid:
            data = request.get_json()
            print(data)
            item.update({'description':data.get('description'), 'price':data.get('price')})
            return jsonify(item), HTTPStatus.OK
    return jsonify({'message': 'article not found'}), HTTPStatus.NOT_FOUND


@app.route('/articles/<int:oid>', methods=['DELETE'])
@jwt_required()
def delete(oid):
    for item in table:
        print(item)
        if item['id'] == oid:
            table.remove(item)
            return '', HTTPStatus.NO_CONTENT
    return jsonify({'message': 'article not found'}), HTTPStatus.NOT_FOUND


if __name__ == '__main__':
    app.run(port=8443, debug=True, ssl_context='adhoc')
