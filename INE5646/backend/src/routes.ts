import { Router } from "express";

import authMiddleware from "./app/middlewares/authMiddleware";

import UserController from "./app/controllers/UserController";
import AuthController from "./app/controllers/AuthController";
import AnimeController from "./app/controllers/AnimeController";

const router = Router();

router.post('/users', UserController.store);
router.post('/auth', AuthController.authenticate);

router.post('/animes', authMiddleware, AnimeController.addAnime);
router.get('/animes', authMiddleware, AnimeController.getAllAnimes);
router.get('/animes/:day', authMiddleware, AnimeController.getAnimeByReleaseDay);
router.put('/animes/watch/:id', authMiddleware, AnimeController.watchEpisode);
router.delete('/animes/:id', authMiddleware, AnimeController.deleteAnime);

export default router;