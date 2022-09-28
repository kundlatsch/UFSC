import React from 'react';
import faker from 'faker';

const CommentDetail = (props) => {
    return (
        <div className="comment">
            <a href="/" className="avatar">
                <img alt="Avatar" src={ faker.image.avatar() }/>
            </a>
            <div className="content">
                <a href="/" className="author">
                    { props.author }
                </a>
                <div className="metadata">
                    <span className="date">{ props.timeAgo + ' ago' }</span>
                </div>
                <div className="text">{ faker.random.words() }</div>
            </div>
        </div>
    );
}

export default CommentDetail;