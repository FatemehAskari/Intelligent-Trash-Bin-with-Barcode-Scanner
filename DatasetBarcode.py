import sqlite3

conn = sqlite3.connect('mqttrecycle.db')
cursor = conn.cursor()

cursor.execute("DROP TABLE IF EXISTS barcodedata")

cursor.execute('''CREATE TABLE IF NOT EXISTS barcodedata
                  (id INTEGER PRIMARY KEY, barcode TEXT, type INTEGER)''')

data = [
    (1, '6930358609225', 0),
    (2, '8690146123637', 0),
    (3, '6260176700829', 1),
    (4, '8901057335645', 1),
    (5, '6001567179851', 0),
    (6, '6260111310106',0)
]

for record in data:
    conn = sqlite3.connect('mqttrecycle.db')
    cursor = conn.cursor()
    cursor.execute('INSERT INTO barcodedata VALUES (?, ?, ?)', record)
    conn.commit()
    conn.close()