INSERT INTO geographical_regions (name, climate, geological_characteristics) VALUES 
('Iceland', 'Cold', 'Volcanic island with frequent eruptions'),
('Hawaii', 'Tropical', 'Home to numerous shield volcanoes'),
('Mount Etna', 'Mediterranean', 'Active stratovolcano'),
('Yellowstone', 'Continental', 'Supervolcano with geothermal features');

INSERT INTO volcanic_events (name, type, impact, region_id) VALUES 
('Eruption of Eyjafjallajökull', 'Explosive', 'Significant ash cloud affecting air travel', 1),
('Mauna Loa Eruption', 'Effusive', 'Lava flows creating new land', 2),
('Mount St. Helens', 'Explosive', 'Major eruption in 1980', 3),
('Krakatoa Eruption', 'Cataclysmic', '1883 eruption caused global climate change', 4);

INSERT INTO geological_formations (name, location, formation_type, region_id) VALUES 
('Basalt Columns', 'Giant’s Causeway', 'Interlocking Basalt', 1),
('Lava Tubes', 'Volcanoes National Park', 'Tunnel-like', 2),
('Caldera of Yellowstone', 'Yellowstone', 'Large volcanic crater', 4),
('Granite Peaks', 'Sierra Nevada', 'Massive granite formations', 3);

INSERT INTO volcanic_rocks (name, event_id, formation_id) VALUES 
('Obsidian', 1, 1),
('Pumice', 2, 2),
('Basalt', 3, 3),
('Rhyolite', 4, 4);

INSERT INTO chemical_compositions (rock_id, element, percentage) VALUES 
(1, 'Silicon', 50.0),
(1, 'Oxygen', 44.0),
(2, 'Aluminum', 8.0),
(2, 'Iron', 5.0);

INSERT INTO researchers (name, affiliation, area_of_expertise) VALUES 
('Dr. Jane Smith', 'University of Geosciences', 'Volcanology'),
('Dr. John Doe', 'Institute of Earth Studies', 'Petrology'),
('Dr. Alice Johnson', 'Global Geology Corp', 'Sedimentology'),
('Dr. Bob Brown', 'Tech University', 'Geochemistry');

INSERT INTO study_projects (title, end_date, objective) VALUES 
('Volcanic Rocks of Iceland', '2023-12-31', 'Study the composition and formation'),
('Hawaiian Volcano Dynamics', '2024-06-30', 'Observe and model lava flows'),
('Yellowstone Geothermal Activity', '2025-08-15', 'Study geysers and hot springs'),
('Andes Mountain Formation', '2023-11-20', 'Understand the geological processes');

INSERT INTO formation_projects (formation_id, project_id) VALUES 
(1, 1),
(2, 2),
(3, 3),
(4, 4);

INSERT INTO researcher_projects (researcher_id, project_id) VALUES 
(1, 1),
(2, 2),
(3, 3),
(4, 4);
