import React from 'react';
import { useHistory } from 'react-router-dom';
import { Formik, Form, Field, ErrorMessage } from 'formik';
import * as Yup from 'yup';
import Swal from 'sweetalert2';
import withReactContent from 'sweetalert2-react-content';

import './styles.css';
import atAPI from '../../services/atAPI';

const Register = () => {

  let history = useHistory();
  const MySwal = withReactContent(Swal);

  const initialValues = {
    Email: "",
    Username: "",
    Password: "",
  }

  const validationSchema = Yup.object().shape({
    Username: Yup.string().required().min(4).max(50),
    Email: Yup.string().required().email(),
    Password: Yup.string().required().min(8),
    RepeatPassword: Yup.string()
      .required('Please confirm your password')
      .oneOf(
        [Yup.ref('Password'), null], 
        'Passwords must match'
      )
  });

  function handleLoginClick() {
    history.push("/login");
  }

  const handleRegister = (data) => {
    atAPI.post('/users', {
      email: data.Email,
      name: data.Username,
      password: data.Password,
    }).then(res => {
      MySwal.fire({
        icon: 'success',
        title: 'Successs!',
        text: 'You can now login with your new account',
      });
      history.push("/login");
    }).catch(err => {
      MySwal.fire({
        icon: 'error',
        title: 'Something went wrong...',
        text: 'This user already exists, please use another email!',
      });
    });
  };

  return (
    <div className="center-container" id="register-page">
      <div className="default-container">
        <h1>Create a new account</h1>
        <Formik
          initialValues={initialValues}
          onSubmit={handleRegister}
          validationSchema={validationSchema}
        >
        <Form className="form-container">

          <div className="vspace5"></div>
          <div className="vspace20">
            <ErrorMessage name="Username" component="span" />
          </div>
          <Field 
            className="default-input register-input"
            name="Username"
            type="text"
            placeholder="Username"
          />

          <div className="vspace5"></div>
          <div className="vspace20">
            <ErrorMessage name="Email" component="span" />
          </div>
          <Field 
            className="default-input register-input"
            name="Email"
            type="email"
            placeholder="Email"
          />
          
          <div className="vspace5"></div>
          <div className="vspace20">
            <ErrorMessage name="Password" component="span" />
          </div>
          <Field 
            className="default-input register-input" 
            name="Password"
            type="password"
            placeholder="Password"
          />

          <div className="vspace5"></div>
          <div className="vspace20">
            <ErrorMessage name="RepeatPassword" component="span" />
          </div>
          <Field 
            className="default-input register-input" 
            name="RepeatPassword"
            type="password"
            placeholder="Confirm Password"
          />

          <button
            type="submit" 
            className="green-button"
            id="register-button"
          >
            Register
          </button>

        </Form>
      </Formik>

        <p onClick={handleLoginClick}>
          Already have an account? Click here to login
        </p>
      </div>
    </div>
   
  )
};

export default Register;