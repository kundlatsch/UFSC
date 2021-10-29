import { Request, Response } from 'express';
import { getRepository } from 'typeorm';
import bcrypt from 'bcryptjs';
import jwt from 'jsonwebtoken';

import User from '../models/User';

class AuthController {

  async authenticate(req: Request, res: Response) {
    const repository = getRepository(User);
    const { email, password } = req.body;

    const user = await repository.findOne({
      where: { email }
    });

    if (!user) {
      return res.sendStatus(409);
    }

    const isValidPassword = await bcrypt.compare(password, user.password);

    if (!isValidPassword) {
      return res.sendStatus(401);
    }

    const token = jwt.sign({ id: user.id }, 'secret-key', { expiresIn: '1d' });

    return res.json({
      id: user.id,
      name: user.name,
      email: user.email,
      token,
    })
  }

}

export default new AuthController();