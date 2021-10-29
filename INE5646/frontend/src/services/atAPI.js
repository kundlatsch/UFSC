import axios from 'axios';

// AnimeTracker API
export default axios.create({
  baseURL: 'http://localhost:3333',
});