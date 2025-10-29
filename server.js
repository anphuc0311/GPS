const express = require('express');
const app = express();
const port = 8080;

// Set the view engine to EJS
app.set('view engine', 'ejs');

// Set the directory where templates are located
app.set('views', './views');

// Route that renders a template
app.get('/', (req, res) => {
  const data = {
    title: 'Express Template Example',
    message: 'Hello from EJS!',
    items: ['Item 1', 'Item 2', 'Item 3']
  };

  // Renders the views/sample.ejs template
  res.render('sample', data);
});

app.listen(port, () => {
  console.log(`Example app listening at http://localhost:${port}`);
});