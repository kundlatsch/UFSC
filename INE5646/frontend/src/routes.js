import React from 'react';
import { Redirect, Route, BrowserRouter, Switch } from 'react-router-dom';

import Home from './pages/Home';
import Register from './pages/Register';
import Login from './pages/Login';
import UserHome from './pages/UserHome';
import FullAnimeList from './pages/FullAnimeList';
import PageNotFound from './pages/PageNotFound';

const Routes = () => {
  return (
    <BrowserRouter>
      <Switch>
        <Route component={Home} path="/" exact />
        <Route component={Register} path="/register" exact />
        <Route component={Login} path="/login" exact />
        <Route component={UserHome} path="/userHome" exact />
        <Route component={FullAnimeList} path="/animeHistory" exact />
        <Route component={PageNotFound} path="/404"/>
        <Redirect to="/404"/>  
      </Switch>
    </BrowserRouter>
  )
};

export default Routes;