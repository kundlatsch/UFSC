import axios from 'axios';

export default axios.create({
    baseURL: 'https://api.unsplash.com',
    headers: {
        Authorization: 'Client-ID 60f6fc3bc95b9f3e82076d3c2184c9d291b727449b975376e21d18fe51f25816'
    }
});