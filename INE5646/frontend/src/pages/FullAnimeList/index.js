import React, { useEffect, useState, useContext } from 'react';
import { FiArrowLeft, FiPlus, FiTrash2 } from 'react-icons/fi';
import { useHistory } from 'react-router-dom';

import './styles.css';
import { Context } from '../../context/AuthContext';
import atAPI from '../../services/atAPI';
import weekday from './daymap';

function FullAnimeList() {

  const [animeList, setAnimeList] = useState([]);
  let history = useHistory();
  const { authenticated, handleLogout } = useContext(Context);

  useEffect(() => {
    if (!authenticated) {
      history.push("/login");
    }
    // TODO: get dayAnime from the backend instead of mock data
    
    atAPI.get(`/animes`).then(res => {
      const { data } = res;
      if (data) {
        setAnimeList(data);
      } else {
        setAnimeList([]);
      }
    });
    
    
    // setAnimeList(animeMockData);
  }, []);

  const goToPreviousPage = () => {
    history.goBack();
  }

  const handlePlusClick = async (anime) => {
    const updateId = anime.id;
    const newCurrentEpisode = anime.currentEpisode + 1;
    
    if (anime.currentEpisode <= anime.totalEpisodes) {
      try {
        await atAPI.put(`animes/watch/${anime.id}`);
      } catch(err) {
        console.log(err);
      }
    }

    const newAnimeList = (animeList.map(animeMap => {
      return animeMap.id === updateId && newCurrentEpisode <= animeMap.totalEpisodes ? 
        { ...animeMap, currentEpisode: newCurrentEpisode }: animeMap
    }));
    setAnimeList(newAnimeList);
  }

  const handleDelClick = (anime) => {
    atAPI.delete(`/animes/${anime.id}`);
    const newAnimeList = animeList.filter((animeFilt) => {
      return animeFilt.id != anime.id;
    })
    setAnimeList(newAnimeList);
  }

  return (
    <div className="center-container">
      <div className="default-container" id="anime-history">
        <div className="back-arrow-container" onClick={goToPreviousPage}>
          <FiArrowLeft size="1.5em"/>
        </div>
        <h2>Your anime history</h2>
        
        <div className="anime-container">
          <div className="anime-container-line-history">
            <div className="anime-container-column header">
              Name
            </div>

            <div className="anime-container-column header">
              Release Day
            </div>

            <div className="anime-container-column header">
              Episodes
            </div>

            <div className="anime-container-column header" id="del">
              
            </div>
          </div>

          {animeList.map(anime => (
            <div className="anime-container-line-history" key={anime.id}>
            <div className="anime-container-column">
              {anime.anime}
            </div>

            <div className="anime-container-column">
              {weekday[anime.releaseDay]}
            </div>

            <div className="anime-container-column">
              {anime.currentEpisode}/{anime.totalEpisodes}
              <div className="plus-container" onClick={() => {handlePlusClick(anime)}}>
                <FiPlus />
              </div>
            </div>
            <div className="anime-container-column" id="del">
              <div className="del-container" onClick={() => {handleDelClick(anime)}}>
                <FiTrash2 color="#e35b5b" />
              </div>
            </div>
          </div>
          ))}
          
        </div>

      </div>  
    </div>
  )
}

export default FullAnimeList;
