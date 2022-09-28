import React from 'react';
import { connect } from 'react-redux';

const renderDetails = song => {
    if (!song) {
        return <div>Select a song!</div>
    }

    return (
        <div>
            <h3>Details for:</h3>
            <p>
                Title: {song.title}
                <br/>   
                Duration: {song.duration}
            </p>
            
        </div>
    );
}

const SongDetail = ({song}) => {
    return (
        <div 
            className="ui raised very padded text segment"
            style={{ height: '150px', backgroundColor: '#edf5f5' }}
        >
            {renderDetails(song)}
        </div>
    );
}

const mapStateToProps = state => ({
    song: state.selectedSong
});

export default connect(mapStateToProps)(SongDetail);