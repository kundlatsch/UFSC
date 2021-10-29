import React from 'react';
import { useHistory } from 'react-router-dom';

function PageNotFound() {
  let history = useHistory();

  return (
    <div>
      <h1 style={{marginTop: "100px", textAlign: "center", color: "#FFF"}}>
          Error 404: Page not found!
      </h1>
      <div style={{width: "100%", display: "flex", justifyContent: "center", marginTop: "50px"}}>
        <button onClick={() => history.push("/userHome")}>Return</button>

      </div>
    </div>
  )
}

export default PageNotFound;