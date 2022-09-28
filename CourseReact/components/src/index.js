import React from 'react';
import ReactDOM from 'react-dom';
import faker from 'faker';
import CommentDetail from './CommentDetail';
import ApprovalCard from './ApprovalCard';

faker.locale = 'pt_BR'

const App = () => {
    return (
        <div className="ui container comments">
            <ApprovalCard>
                <div>
                    <h4>Warning!</h4>
                    <p>Are you sure you want to do this?</p>
                </div>
            </ApprovalCard>
            <ApprovalCard>
                <CommentDetail author="João" timeAgo="3 Hours" />
            </ApprovalCard>
            
            <ApprovalCard>
                <CommentDetail author="Gustavo" timeAgo="1 Day" />
            </ApprovalCard>
            
            <ApprovalCard>
                <CommentDetail author="Barnei" timeAgo="10 Hours"/>
            </ApprovalCard>
            
        </div>
    );
};

ReactDOM.render(<App />, document.querySelector('#root'));