-- Executed by the MariaDB container on the first start (empty volume)
CREATE TABLE messages
(
    address  INT          NOT NULL PRIMARY KEY,
    priority INT          NOT NULL,
    data     VARCHAR(255) NOT NULL
) ENGINE = INNODB;

INSERT INTO messages (address, priority, data) VALUES (1123, 2, 'voltage=3.4V');
INSERT INTO messages (address, priority, data) VALUES (1124, 2, 'voltage=1.3V');
INSERT INTO messages (address, priority, data) VALUES (1125, 1, 'voltage=-1.7V');
