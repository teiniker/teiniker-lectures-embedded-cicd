import os
import pymysql
from pymysql.constants import CLIENT
from message import Message


class MessageDao:
    """Data Access Object for the messages MariaDB table."""

    def __init__(self) -> None:
        # The connection parameters are passed as environment variables
        # (see docker-compose.yml)
        self.config = {
            'host': os.environ.get('DB_HOST', 'localhost'),
            'port': int(os.environ.get('DB_PORT', '3306')),
            'user': os.environ.get('DB_USER', 'student'),
            'password': os.environ.get('DB_PASSWORD', 'student'),
            'database': os.environ.get('DB_NAME', 'testdb'),
            'autocommit': True,
            # Report matched rows (not changed rows) for UPDATE, so an
            # UPDATE with unchanged values still returns rowcount 1
            'client_flag': CLIENT.FOUND_ROWS,
        }

    def _connect(self) -> pymysql.connections.Connection:
        return pymysql.connect(**self.config)

    # CRUD operations

    def find_all(self) -> list:
        with self._connect() as conn, conn.cursor() as cursor:
            cursor.execute('SELECT address, priority, data FROM messages')
            rows = cursor.fetchall()
        return [Message(address=r[0], priority=r[1], data=r[2]) for r in rows]

    def find_by_id(self, address: int) -> Message | None:
        with self._connect() as conn, conn.cursor() as cursor:
            cursor.execute(
                'SELECT address, priority, data FROM messages '
                'WHERE address = %s',
                (address,)
            )
            row = cursor.fetchone()
        if row is None:
            return None
        return Message(address=row[0], priority=row[1], data=row[2])

    def insert(self, dto: Message) -> Message:
        with self._connect() as conn, conn.cursor() as cursor:
            cursor.execute(
                'INSERT INTO messages (address, priority, data) '
                'VALUES (%s, %s, %s)',
                (dto.address, dto.priority, dto.data)
            )
        return dto

    def update(self, address: int, dto: Message) -> Message | None:
        with self._connect() as conn, conn.cursor() as cursor:
            count = cursor.execute(
                'UPDATE messages SET priority = %s, data = %s '
                'WHERE address = %s',
                (dto.priority, dto.data, address)
            )
        if count == 0:
            return None
        return Message(address=address, priority=dto.priority, data=dto.data)

    def delete(self, address: int) -> bool:
        with self._connect() as conn, conn.cursor() as cursor:
            count = cursor.execute(
                'DELETE FROM messages WHERE address = %s',
                (address,)
            )
        return count > 0
