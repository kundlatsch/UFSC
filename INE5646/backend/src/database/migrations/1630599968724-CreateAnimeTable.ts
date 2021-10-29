import {MigrationInterface, QueryRunner, Table, TableForeignKey} from "typeorm";

export class CreateAnimeTable1630599968724 implements MigrationInterface {

  public async up(queryRunner: QueryRunner): Promise<void> {

    await queryRunner.query('CREATE EXTENSION IF NOT EXISTS "uuid-ossp"');

    await queryRunner.createTable(new Table({
      name: 'Animes',
      columns: [
        {
          name: 'id',
          type: 'uuid',
          isPrimary: true,
          generationStrategy: 'uuid',
          default: 'uuid_generate_v4()',
        },
        {
          name: 'anime',
          type: 'varchar',
        },
        {
          name: 'totalEpisodes',
          type: 'int',
        },
        {
          name: 'currentEpisode',
          type: 'int',
        },
        {
          name: 'releaseDay',
          type: 'int',
        },
        {
          name: 'userId',
          type: 'uuid',
        },
      ],
    }));

    await queryRunner.createForeignKey(
      'Animes',
      new TableForeignKey({
          columnNames: ['userId'],
          referencedColumnNames: ['id'],
          referencedTableName: 'Users',
          onDelete: 'CASCADE',
      }),
    );

  }

  public async down(queryRunner: QueryRunner): Promise<void> {
    await queryRunner.dropTable('Animes');
    await queryRunner.query('DROP EXTENSION "uuid-ossp"');
  }

}
