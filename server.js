const http = require('http');
const path = require('path');
const fs = require('fs');
const qs = require('querystring');
const checkPrivileges = require('./build/Release/addon');

const PORT = 3000;

const server = http.createServer((req, res) => {
    const pathname = new URL(req.url, `http://${req.headers.host}`).pathname;

    if (req.method === 'GET' && pathname === '/') {        
        res.writeHead(200, { 'Content-Type': 'text/html' });
        fs.createReadStream(path.join(__dirname, 'public', 'index.html')).pipe(res);

    } else if (req.method === 'POST' && pathname === '/') {
        let body = '';
        
        req.on('data', (chunk) => {
            body += chunk.toString();
        });
        
        req.on('end', () => {
            const { username } = qs.parse(body);
            
            if (checkPrivileges && typeof checkPrivileges.checkPrivileges === 'function') {
                const privilege = checkPrivileges.checkPrivileges(username);
                
                if (privilege) {
                    res.writeHead(200, { 'Content-Type': 'text/plain; charset=utf-8' });
                    res.end(`Пользователь ${username} имеет привилегию ${privilege}`);

                } else {
                    res.writeHead(200, { 'Content-Type': 'text/plain; charset=utf-8' });
                    res.end(`Пользователя ${username} нет`);
                }
            } else {
                res.writeHead(200, { 'Content-Type': 'text/plain; charset=utf-8' });
                res.end(`Ошибка: функция checkPrivileges не найдена`);
            }
        });
    } else {
        res.writeHead(404, { 'Content-Type': 'text/plain; charset=utf-8' });
        res.end('Страница не найдена');
    }
});

// Запуск сервера
server.listen(PORT, () => { 
  console.log("Сервер запущен по адресу http://localhost:3000")
});