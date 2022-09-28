import React from 'react';
import SearchBar from './SearchBar';
import youtube from '../apis/youtube';
import VideoList from './VideoList';
import VideoDetail from './VideoDetail';
import { throwStatement } from '@babel/types';

class App extends React.Component {

    state = { videos: [], selectedVideo: null };

    componentDidMount () {
        this.onTermSubmit('React.js');
    }

    onTermSubmit = async (term) => {
        const response = await youtube.get('/search', {
            params: {
                q: term
            }
        });

        this.setState({ 
            videos: response.data.items,
            selectedVideo: response.data.items[1]
        });
    };

    onVideoSelect = (video) => {
        this.setState({ selectedVideo: video });
    };

    render () {
        return (
            <div className="ui container">
                <SearchBar submit={this.onTermSubmit}/>
                <div className="ui grid">
                    <div className="ui row">
                        <div className="eleven wide column">     
                            <VideoDetail video={this.state.selectedVideo}/>
                        </div>
                        <div className="five wide column">
                            <VideoList
                                onVideoSelect={this.onVideoSelect}
                                videos={ this.state.videos }
                            />
                        </div>
                    </div>
                </div>
            </div>
        );
    }
}

export default App;