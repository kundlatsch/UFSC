import React, { useState, useContext } from 'react'
import { useHistory } from 'react-router-dom';
import validator from 'validator';
import Swal from 'sweetalert2';
import withReactContent from 'sweetalert2-react-content';

import { Context } from '../../context/AuthContext';

import './styles.css';

function Login() {

  let history = useHistory();
  const { authenticated, handleLogin } = useContext(Context);
  const MySwal = withReactContent(Swal);

  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");

  function handleRegisterClick() {
    history.push("/register");
  }

  const handleLoginClick = () => {
    if (!validator.isEmail(email)) {
      MySwal.fire({
        icon: 'error',
        title: 'Something went wrong...',
        text: 'Please enter a valid email!',
      });
    } else if (!password) {
      MySwal.fire({
        icon: 'error',
        title: 'Something went wrong...',
        text: 'Password is a required field!',
      });
    } else {
      handleLogin(email, password).then((data) => {
        if (!data) {
          MySwal.fire({
            icon: 'error',
            title: 'Something went wrong...',
            text: 'Login failed, please verify your email and password values!',
          });
        } else {
          history.push('/userHome');
        }
      });
    }
  };

  return (
    <div className="center-container" id="login-page">
      <div className="default-container">
        <h1>Login</h1>
        <input 
          type="email"
          className="default-input"
          id="login-input"
          placeholder="E-mail"
          onChange={(e) => {
            setEmail(e.target.value);
          }}
        />
        <input 
          type="password"
          className="default-input"
          placeholder="Password"
          onChange={(e) => {
            setPassword(e.target.value);
          }}
        />

        <button
          className="green-button"
          id="login-button"
          onClick={handleLoginClick}
        >
          Login
        </button>

        <p onClick={handleRegisterClick}>
          Don't have an account? Click here to register
        </p>
      </div>
    </div>
    
  )
}

export default Login