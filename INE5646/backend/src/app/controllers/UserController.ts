import { Request, Response } from 'express';
import { getRepository } from 'typeorm';
import User from '../models/User';

class UserController {

  async store(req: Request, res: Response) {
    const repository = getRepository(User);
    const { name, email, password } = req.body;

    const userExist = await repository.findOne({
      where: { email }
    });

    if (userExist) {
      return res.sendStatus(409);
    }

    const user = repository.create({ name, email, password });
    
    await repository.save(user);
    return res.json(user);
  }

}

export default new UserController();