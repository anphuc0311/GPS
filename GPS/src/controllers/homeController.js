

const getHomePage = (req, res) => {
    res.send('Hello from Home Controller!');
}


const test = (req, res) => {
    const data = {
    title: 'Express Template Example',
    message: 'Hello from EJS!',
    items: ['Item 4', 'Item 2', 'Item 3']
  };

  // Renders the views/sample.ejs template 
  res.render('sample', data);
}
module.exports = {
    getHomePage,
    test
};