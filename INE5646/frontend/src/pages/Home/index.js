import React from 'react';
import { useHistory } from "react-router-dom";

import logo from './logo.png';
import nen from './nen.png';
import './styles.css';

const Home = () => {

  let history = useHistory();

  function handleLoginClick() {
    history.push("/login");
  }

  function handleRegisterClick() {
    history.push("/register");
  }

  return (
    <div className="home-page">
      <div className="left-container">

        <img id="logo" src={logo} alt="Anime Tracker" />

        <h1>Keep track of your anime</h1>
        <h1>anywhere, anytime</h1>

        <div className="button-container">
          <button className="white-button" onClick={handleRegisterClick}>
            Register
          </button>
          <button className="green-button" onClick={handleLoginClick}>
            Login
          </button>
        </div>

      </div>

      <div className="right-container">
        <img id="nen" src={nen} alt="Nen" />
      </div>
    </div>
   
  )
};

export default Home;