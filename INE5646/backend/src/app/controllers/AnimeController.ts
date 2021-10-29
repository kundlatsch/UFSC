import { Request, Response } from 'express';
import { getRepository } from 'typeorm';
import Anime from '../models/Animes';

class AnimeController {

  async addAnime(req: Request, res: Response) {
    const repository = getRepository(Anime);
    const { anime, totalEpisodes, releaseDay } = req.body;
    const userId = req.userId;

    const animeObj = {
      anime,
      totalEpisodes,
      currentEpisode: 0,
      releaseDay,
    }

    const newAnime = repository.create({
      ...animeObj,
      userId
    });
    
    await repository.save(newAnime).catch(err => {
      console.log(err);
      return res.sendStatus(400);
    });

    return res.json({
      ...animeObj,
      id: newAnime.id
    });
  }

  async getAllAnimes(req: Request, res: Response) {
    const repository = getRepository(Anime);
    const userId = req.userId;

    const animes = await repository.find({
      where: { userId }
    });
    
    if (animes.length === 0) {
      return res.sendStatus(204);
    }

    const returnAnimes = animes.map(anime => {
      return {
        id: anime.id,
        anime: anime.anime,
        totalEpisodes: anime.totalEpisodes,
        releaseDay: anime.releaseDay,
        currentEpisode: anime.currentEpisode,
      }
    });

    return res.json(returnAnimes);
  }

  async getAnimeByReleaseDay(req: Request, res: Response) {
    const repository = getRepository(Anime);
    const releaseDay = req.params.day;
    const userId = req.userId;

    const animes = await repository.find({
      where: { userId, releaseDay }
    });

    if (animes.length === 0) {
      return res.sendStatus(204);
    }
    
    const returnAnimes = animes.map(anime => {
      return {
        id: anime.id,
        anime: anime.anime,
        totalEpisodes: anime.totalEpisodes,
        currentEpisode: anime.currentEpisode,
      }
    });

    return res.json(returnAnimes);
  }

  async watchEpisode(req: Request, res: Response) {
    const repository = getRepository(Anime);
    const id = req.params.id;
    const userId = req.userId;

    const anime = await repository.findOne({
      where: { userId, id }
    });

    if (!anime) {
      return res.sendStatus(400);
    }

    if (anime.totalEpisodes < anime.currentEpisode + 1) {
      return res.sendStatus(403);
    }

    const updatedAnime = {
      ...anime,
      currentEpisode: anime.currentEpisode + 1
    };
    
    await repository.save(updatedAnime);

    return res.json(updatedAnime);
  }

  async deleteAnime(req: Request, res: Response) {
    const repository = getRepository(Anime);
    const id = req.params.id;
    const userId = req.userId;
    
    const { affected } = await repository.delete({
      id,
      userId,
    });

    if (affected === 0) {
      return res.sendStatus(400);
    }

    return res.sendStatus(200);
  }
  
}

export default new AnimeController();