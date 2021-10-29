import { Column, Entity, PrimaryGeneratedColumn } from 'typeorm';

@Entity('Animes')
class Anime {
  @PrimaryGeneratedColumn('uuid')
  id: string;
  
  @Column()
  anime: string;

  @Column()
  totalEpisodes: number;

  @Column()
  currentEpisode: number;

  @Column()
  releaseDay: number;

  @Column()
  userId: string;

}

export default Anime;