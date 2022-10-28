--
-- Файл сгенерирован с помощью SQLiteStudio v3.3.3 в ср окт. 19 13:22:57 2022
--
-- Использованная кодировка текста: UTF-8
--
PRAGMA foreign_keys = off;
BEGIN TRANSACTION;

-- Таблица: Checks
CREATE TABLE Checks (
    id          INTEGER PRIMARY KEY AUTOINCREMENT
                        NOT NULL,
    contract    INTEGER REFERENCES SaleContracts (id) ON DELETE RESTRICT
                                                      ON UPDATE CASCADE
                        NOT NULL,
    total_price REAL    NOT NULL,
    paid        BOOLEAN NOT NULL
                        DEFAULT (false),
    date        DATE    NOT NULL
                        DEFAULT (CURRENT_DATE)
);


-- Таблица: Clients
CREATE TABLE Clients (
    id      INTEGER PRIMARY KEY AUTOINCREMENT
                    NOT NULL,
    name    TEXT    NOT NULL,
    address TEXT,
    phone   TEXT    NOT NULL,
    fax     TEXT,
    account TEXT    NOT NULL,
    notes   TEXT
);


-- Таблица: Payments
CREATE TABLE Payments (
    id          INTEGER PRIMARY KEY AUTOINCREMENT
                        NOT NULL,
    contract    INTEGER REFERENCES SupplyContracts (id) ON DELETE RESTRICT
                                                        ON UPDATE CASCADE
                        NOT NULL,
    total_price REAL    NOT NULL,
    paid        BOOLEAN NOT NULL
                        DEFAULT (false),
    date        DATE    NOT NULL
                        DEFAULT (CURRENT_DATE)
);


-- Таблица: Products
CREATE TABLE Products (
    id         INTEGER PRIMARY KEY AUTOINCREMENT,
    type       INTEGER REFERENCES ProductTypes (id) ON DELETE RESTRICT
                                                    ON UPDATE CASCADE,
    name       TEXT    NOT NULL,
    properties TEXT,
    price      REAL    NOT NULL,
    package    TEXT    NOT NULL,
    supply     TEXT,
    count      INTEGER NOT NULL
                       DEFAULT (0)
);


-- Таблица: ProductsSale
CREATE TABLE ProductsSale (
    id       INTEGER PRIMARY KEY AUTOINCREMENT
                     NOT NULL,
    product  INTEGER REFERENCES Products (id) ON DELETE RESTRICT
                                              ON UPDATE CASCADE
                     NOT NULL,
    count    INTEGER NOT NULL,
    contract INTEGER REFERENCES SaleContracts (id) ON DELETE RESTRICT
                                                   ON UPDATE CASCADE
                     NOT NULL,
    shipped  BOOLEAN NOT NULL
                     DEFAULT (false)
);


-- Таблица: ProductsSupply
CREATE TABLE ProductsSupply (
    id       INTEGER PRIMARY KEY AUTOINCREMENT
                     NOT NULL,
    product  INTEGER REFERENCES Products (id) ON DELETE RESTRICT
                                              ON UPDATE CASCADE
                     NOT NULL,
    count    INTEGER NOT NULL,
    contract INTEGER REFERENCES SupplyContracts (id) ON DELETE RESTRICT
                                                     ON UPDATE CASCADE
                     NOT NULL,
    received BOOLEAN NOT NULL
                     DEFAULT (false)
);


-- Таблица: ProductTypes
CREATE TABLE ProductTypes (
    id   INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT    NOT NULL
);


-- Таблица: SaleContracts
CREATE TABLE SaleContracts (
    id     INTEGER PRIMARY KEY AUTOINCREMENT
                   NOT NULL,
    date   DATE    NOT NULL
                   DEFAULT (CURRENT_DATE),
    text   TEXT    NOT NULL,
    client INTEGER REFERENCES Clients (id) ON DELETE RESTRICT
                                           ON UPDATE CASCADE
                   NOT NULL
);


-- Таблица: SupplyContracts
CREATE TABLE SupplyContracts (
    id     INTEGER PRIMARY KEY AUTOINCREMENT
                   NOT NULL,
    date   DATE    NOT NULL
                   DEFAULT (CURRENT_DATE),
    text   TEXT    NOT NULL,
    vendor INTEGER REFERENCES Vendors (id) ON DELETE RESTRICT
                                           ON UPDATE CASCADE
                   NOT NULL
);


-- Таблица: Vendors
CREATE TABLE Vendors (
    id           INTEGER PRIMARY KEY AUTOINCREMENT
                         NOT NULL,
    name         TEXT    NOT NULL,
    address      TEXT,
    manager      TEXT    NOT NULL,
    accountant   TEXT    NOT NULL,
    account_info TEXT    NOT NULL
);


COMMIT TRANSACTION;
PRAGMA foreign_keys = on;
