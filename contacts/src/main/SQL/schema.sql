DROP TABLE contacts;

CREATE TABLE public.contacts
(
  id bigint NOT NULL,
  firstName character varying(30) NOT NULL,
  lastName character varying(50) NOT NULL,
  phoneNumber character varying(13),
  emailAddress character varying(30),
  CONSTRAINT users_pkey PRIMARY KEY (id)
);