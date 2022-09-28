// Import the React and React-dom libraries
import React from 'react';
import ReactDOM from 'react-dom';


// Create a react component
// function can be replace by the arrow function
// notation as: App = () => { return; }
const App = function () {
    return <div>Hello world!</div>
}


// Take the react component and show it on the screen
ReactDOM.render(
    <App />, 
    document.querySelector('#root')
);