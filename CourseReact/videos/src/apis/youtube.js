import axios from 'axios';

const KEY = 'AIzaSyDW6udTf-7wSrEtvf2dcnLO9C4v4C2GaLc'

export default axios.create({
    baseURL: 'https://www.googleapis.com/youtube/v3/',
    params: {
        part: 'snippet',
        maxResults: 5,
        key: KEY
    }
});