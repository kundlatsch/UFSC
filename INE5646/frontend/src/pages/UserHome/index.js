import React, { useState, useEffect, useContext } from 'react';
import { FiPlus } from 'react-icons/fi';
import NewAnimeModal from './components/NewAnimeModal';
import { useHistory } from 'react-router-dom';

import './styles.css';
import { Context } from '../../context/AuthContext';
import atAPI from '../../services/atAPI';


function UserHome() {

  const { authenticated, globalUsername, handleLogout } = useContext(Context);

  const [username, setUsername] = useState("");
  const [weekDay, setWeekday] = useState("");
  const [weekDayNumber, setWeekdayNumber] = useState();
  const [dayAnime, setDayAnime] = useState([]);
  const [dialogOpen, setDialogOpen] = useState(false);

  let history = useHistory();

  const handleClickDialogOpen = () => {
    setDialogOpen(true);
  };

  const handleDialogClose = () => {
    setDialogOpen(false);
  };

  const handleDialogReturn = (newAnime) => {
    if (newAnime.animeWeekDay == weekDayNumber) {
      setDayAnime([...dayAnime, newAnime]);
    }
  }

  const navigateToHistory = () => {
    history.push("/animeHistory");
  };

  const getAnimeData = async () => {
    const browserWeekDayNumber = new Date().getDay();
    setWeekdayNumber(browserWeekDayNumber);
    try {
      const { data } = await atAPI.get(`/animes/${browserWeekDayNumber}`);
      if (!data) {
        return [];
      }
      return data;
    } catch(err) {
      console.log(err);
      return [];
    }
  }

  useEffect(() => {
    if (!authenticated) {
      history.push("/login");
    }
    setUsername(globalUsername);
    const browserWeekDay = new Date().toLocaleString('en-us', {  weekday: 'long' });
    setWeekday(browserWeekDay);
    
    getAnimeData().then(data => setDayAnime(data));
    
  }, []);

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

    const updateAnimeList = (dayAnime.map(animeMap => {
      return animeMap.id === updateId && newCurrentEpisode <= animeMap.totalEpisodes ? 
        { ...animeMap, currentEpisode: newCurrentEpisode }: animeMap
    }));
    setDayAnime(updateAnimeList);
  }

  const handleLogoutClick = () => {
    handleLogout();
    history.push("/");
  }

  return (
    <div className="center-container" id="user-home">
      <div className="default-container">

        <div className="buttons-container">
          <button
            type="submit" 
            className="green-button"
            onClick={handleClickDialogOpen}
          >
            New Anime
          </button>

          <NewAnimeModal 
            open={dialogOpen} 
            handleClose={handleDialogClose} 
            returnFunction={handleDialogReturn}
          />

          <button
            type="submit" 
            className="green-button"
            onClick={navigateToHistory}
          >
            Full List
          </button>
        </div>

        <h2>{username}, these are your {weekDay} episodes:</h2>

        <div className="anime-container">
          <div className="anime-container-line">
            <div className="anime-container-column header">
              Name
            </div>

            <div className="anime-container-column header">
              Episodes
            </div>
          </div>

          {dayAnime.map(anime => (
            <div className="anime-container-line" key={anime.id}>
            <div className="anime-container-column">
              {anime.anime}
            </div>

            <div className="anime-container-column">
              {anime.currentEpisode}/{anime.totalEpisodes}
              <div className="plus-container" onClick={() => {handlePlusClick(anime)}}>
                <FiPlus />
              </div>
            </div>
          </div>
          ))}
          
        </div>

        <p onClick={handleLogoutClick}>
          Logout
        </p>

      </div>
    </div>
  )
}

export default UserHome
