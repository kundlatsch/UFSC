import React, { createContext, useEffect, useState } from "react";

import atAPI from '../services/atAPI';

const Context = createContext();

function AuthProvider({ children }) {

  const [ authenticated, setAuthenticated ] = useState(false);
  const [ globalUsername, setGlobalUsername ] = useState("");
  const [ loading, setLoading ] = useState(true);

  useEffect(() => {
    const token = localStorage.getItem('token');
    if(token) {
      atAPI.defaults.headers.Authorization = `Bearer ${JSON.parse(token)}`;
      const name = localStorage.getItem('name');
      setGlobalUsername(name);
      setAuthenticated(true);
    }
    setLoading(false);
  }, []);

  

  async function handleLogin(email, password) {
    const { data } = await atAPI.post('/auth', {
      email,
      password,
    }).catch(err => {
      return "";
    });
    if (!data) {
      console.log(data);
      return undefined;
    } else {
      const { token, name } = data;
      localStorage.setItem('name', name);
      setGlobalUsername(name);
      localStorage.setItem('token', JSON.stringify(token));
      atAPI.defaults.headers.Authorization = `Bearer ${token}`;
      setAuthenticated(true);
      return true;
    }
    
  }

  function handleLogout() {
    setAuthenticated(false);
    setGlobalUsername("");
    localStorage.removeItem('token');
    localStorage.removeItem('name');
    atAPI.defaults.headers.Authorization = undefined;
  }

  if (loading) {
    return <h1>...</h1>;
  }

  return (
    <Context.Provider value={{ authenticated, globalUsername, handleLogin, handleLogout }}>
      {children}
    </Context.Provider>
  );
}

export { Context, AuthProvider };

