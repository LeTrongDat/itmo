 CREATE TABLE IF NOT EXISTS geographical_regions (
    region_id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL CHECK (name = TRIM(name)),
    climate VARCHAR(255) CHECK (climate IS NULL OR climate = TRIM(climate)),
    geological_characteristics TEXT CHECK (geological_characteristics IS NULL OR geological_characteristics = TRIM(geological_characteristics))
);

CREATE TABLE IF NOT EXISTS volcanic_events (
    event_id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL CHECK (name = TRIM(name)),
    date DATE DEFAULT CURRENT_DATE,
    type VARCHAR(255) CHECK (type IS NULL OR type = TRIM(type)),
    impact TEXT CHECK (impact IS NULL OR impact = TRIM(impact)),
    region_id INTEGER REFERENCES geographical_regions(region_id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS geological_formations (
    formation_id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL CHECK (name = TRIM(name)),
    location VARCHAR(255) CHECK (location IS NULL OR location = TRIM(location)),
    formation_type VARCHAR(255) CHECK (formation_type IS NULL OR formation_type = TRIM(formation_type)),
    discovery_date DATE DEFAULT CURRENT_DATE,
    region_id INTEGER REFERENCES geographical_regions(region_id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS volcanic_rocks (
    rock_id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL CHECK (name = TRIM(name)),
    formation_date DATE DEFAULT CURRENT_DATE,
    event_id INTEGER REFERENCES volcanic_events(event_id) ON DELETE SET NULL ON UPDATE CASCADE,
    formation_id INTEGER REFERENCES geological_formations(formation_id) ON DELETE SET NULL ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS chemical_compositions (
    composition_id SERIAL PRIMARY KEY,
    rock_id INTEGER REFERENCES volcanic_rocks(rock_id) ON DELETE CASCADE ON UPDATE CASCADE,
    element VARCHAR(255) NOT NULL CHECK (element = TRIM(element)),
    percentage NUMERIC CHECK (percentage >= 0 AND percentage <= 100)
);

CREATE TABLE IF NOT EXISTS researchers (
    researcher_id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL CHECK (name = TRIM(name)),
    affiliation VARCHAR(255) CHECK (affiliation IS NULL OR affiliation = TRIM(affiliation)),
    area_of_expertise TEXT CHECK (area_of_expertise IS NULL OR area_of_expertise = TRIM(area_of_expertise))
);

CREATE TABLE IF NOT EXISTS study_projects (
    project_id SERIAL PRIMARY KEY,
    title VARCHAR(255) NOT NULL CHECK (title = TRIM(title)),
    start_date DATE DEFAULT CURRENT_DATE,
    end_date DATE,
    objective TEXT CHECK (objective IS NULL OR objective = TRIM(objective))
);

CREATE TABLE IF NOT EXISTS formation_projects (
    fp_id SERIAL PRIMARY KEY,
    formation_id INTEGER REFERENCES geological_formations(formation_id) ON DELETE CASCADE ON UPDATE CASCADE,
    project_id INTEGER REFERENCES study_projects(project_id) ON DELETE CASCADE ON UPDATE CASCADE,
    create_date DATE DEFAULT CURRENT_DATE,
    UNIQUE (formation_id, project_id)
);

CREATE TABLE IF NOT EXISTS researcher_projects (
    rp_id SERIAL PRIMARY KEY,
    researcher_id INTEGER REFERENCES researchers(researcher_id) ON DELETE CASCADE ON UPDATE CASCADE,
    project_id INTEGER REFERENCES study_projects(project_id) ON DELETE CASCADE ON UPDATE CASCADE,
    create_date DATE DEFAULT CURRENT_DATE,
    UNIQUE (researcher_id, project_id)
);
